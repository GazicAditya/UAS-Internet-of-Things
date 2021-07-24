from typing import Counter
import cv2
import time, sched
import cayenne.client
import logging

MQTT_USERNAME  = "ccb59770-c5fe-11eb-883c-638d8ce4c23d"
MQTT_PASSWORD  = "78ba3bb616b838137acb9b8d6e6c94d36677bec3"
MQTT_CLIENT_ID = "86d83fe0-c604-11eb-883c-638d8ce4c23d"

vidCap = cv2.VideoCapture('Videos/Traffic - 2.mp4')
client = cayenne.client.CayenneMQTTClient()
client.begin(MQTT_USERNAME, MQTT_PASSWORD, MQTT_CLIENT_ID)

#Open Folder
file = open("count.txt", "w+")
file.truncate(0)

#Initialize background substractor for KNN and MOG2
BS_KNN = cv2.createBackgroundSubtractorKNN()
BS_MOG2 = cv2.createBackgroundSubtractorMOG2()
vehicle = 0
while vidCap.isOpened():

    client.loop

    ret, frame1 = vidCap.read() # reads the next frame

    ret, frame = vidCap.read() # reads the next frame
    # Extract KNN method of Foreground Mask
    fgMask = BS_MOG2.apply(frame)

    # reference line
    cv2.line(frame, (140, 440), (1150, 440), (0,255,0), 2)
    cv2.line(frame, (140, 450), (1150, 450), (0,0,255), 2) #red line
    cv2.line(frame, (140, 460), (1150, 460), (0,255,0), 2)

    #extract contours
    conts, _ = cv2.findContours(fgMask, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)

    for c in conts:
        #ignore small contours
        if cv2.contourArea(c) < 1000:
            continue

        x, y, w, h = cv2.boundingRect(c)
        if x > 250 and x < 900 and y > 250:

            #draw bounding rectangle
            cv2.rectangle(frame, (x,y), (x+w, y+h), (0,255,0), 2)
            xMid = int((x + (x+w))/2)
            yMid = int((y + (y+h))/2)

            cv2.circle(frame, (xMid,yMid),5,(0,0,255),5)

            if yMid > 440 and yMid < 460:
                vehicle+=1

    cv2.imshow('Foreground Mask', fgMask)
    cv2.putText(frame, 'Total Vehicles : {}'.format(vehicle), (450, 50), cv2.FONT_HERSHEY_PLAIN, 2, (0,0,0), 2)
    cv2.imshow('Original', frame)

    # wait for any key to be pressed
    if cv2.waitKey(1) & 0xFF == ord('q'):
        file.write(str(vehicle))
        break

client.virtualWrite(1, vehicle, int, "cars")

file.close()
# release vid capture
cv2.destroyAllWindows()
vidCap.release()
