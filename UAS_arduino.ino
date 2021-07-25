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
