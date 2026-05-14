#include <Arduino.h>

#include "OTAHandler.h"
#include "Sensors.h"

//Avoid blocking delays
//Wi-Fi requiers frequent updates in loop()
//Keep constructors light due to hardware limits on startup

//Pin numbers
constexpr uint8_t sensor_left_Pin;
constexpr uint8_t sensor_middle_Pin;
constexpr uint8_t sensor_right_Pin;

OTAHandler otaHandler;

//each sensor is a separate object, test interval and sampling values
opticalSensor leftSensor(sensor_left_Pin);
opticalSensor middleSensor(sensor_middle_Pin);
opticalSensor rightSensor(sensor_right_Pin);



void setup() {
    Serial.begin(115200);

    delay(50); //Short startup delay for WiFi/CPU stabilization
    
    //!!!! Add wifi LAN info before use!
    otaHandler.begin(
    "", //ssid
    "", //password 
    ""  //hostname
    );

    leftSensor.begin();
    middleSensor.begin();
    rightSensor.begin();
}

void loop() {
    otaHandler.update();

    leftSensor.update();
    middleSensor.update();
    rightSensor.update();

    const float leftDistance = leftSensor.getDistanceCm();
    const float middleDistance = middleSensor.getDistanceCm();
    const float rightDistance = rightSensor.getDistanceCm();

    //TODO: Implement LED logic.
    
}