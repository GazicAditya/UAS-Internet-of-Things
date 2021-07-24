/*
Cayenne 2 State Widget Example

This sketch shows how to set up a 2 State Custom Widget with Cayenne.

The CayenneMQTT Library is required to run this sketch. If you have not already done so you can install it from the Arduino IDE Library Manager.

Steps:
1. Attach a digital input device (e.g. a button) to a digital pin on your Arduino.
2. Set the SENSOR_PIN value below to the pin number you used when connecting the sensor.
3. Set the VIRTUAL_CHANNEL value below to a free virtual channel (or the virtual channel of a 2 State Custom Widget you have added) in the Dashboard.
4. Set the Cayenne authentication info to match the authentication info from the Dashboard.
5. Compile and upload this sketch.
6. Once the Arduino connects to the Dashboard it should automatically create a temporary display widget (or update the 2 State widget you have added) with data.
   To make a temporary widget permanent click the plus sign on the widget.
*/

#define CAYENNE_PRINT Serial  // Comment this out to disable prints and save space
#include <CayenneMQTTEthernet.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "ccb59770-c5fe-11eb-883c-638d8ce4c23d";
char password[] = "78ba3bb616b838137acb9b8d6e6c94d36677bec3";
char clientID[] = "86d83fe0-c604-11eb-883c-638d8ce4c23d";

#define VIRTUAL_CHANNEL 1
#define SENSOR_PIN 5 // Do not use digital pins 0 or 1 since those conflict with the use of Serial.
static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;

TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);


void setup()
{
  Serial.begin(9600);
  ss.begin(GPSBaud);
  Cayenne.begin(username, password, clientID);
}

void loop()
{
  Cayenne.loop();
  int sensorValue = analogRead(A0);
  if(sensorValue < 500){  
    Serial.print("Rain - "); Serial.print(gps.location.lat(), 6); Serial.print(", "); Serial.println(gps.location.lng(), 6);
  }else{
    Serial.print("Clear - "); Serial.print(gps.location.lat(), 6); Serial.print(", "); Serial.println(gps.location.lng(), 6);
  }
}

// This function is called at intervals to send sensor data to Cayenne.
CAYENNE_OUT(VIRTUAL_CHANNEL)
{
  // Read data from the sensor and send it to the virtual channel here.
  // For example, to send a digital value you can use the following:
  int value = digitalRead(SENSOR_PIN);
  Cayenne.virtualWrite(VIRTUAL_CHANNEL, value, TYPE_DIGITAL_SENSOR, UNIT_DIGITAL);
}
