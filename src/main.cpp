#include <Arduino.h>
// DO NOT USE delay(), OTA is NOT HAPPY WITH IT
// keep constructors light, hardwere limts on startup

#include "OTAHandler.h"
#include "Sensors.h"

//Pin numbers!!
const int sensorPin1;
const int sensorPin2;

OTAHandler otaHandler;

//each sensor is a separate object
//test interval and sampling values
opticalSensor opticalSensor1(sensorPin1);
opticalSensor opticalSensor2(sensorPin2);

float distance1;
float distance2;

void setup() {

    Serial.begin(115200);

    delay(50); //Give the cpu time to boot up before init of OTA and wifi. Exception to the rule above
    
    otaHandler.begin(
    "", //ssid
    "",//password 
    ""//hostname
    );

    opticalSensor1.begin();
    opticalSensor2.begin();
}

void loop() {
    otaHandler.update();

    opticalSensor1.update();
    opticalSensor2.update();

    distance1 = opticalSensor1.getDistanceCm();
    distance2 = opticalSensor2.getDistanceCm();

    //add LED logic here

}