#include <Arduino.h>

#include "OTAHandler.h"
#include "Sensors.h"

// Avoid long blocking delays.
// OTA requires frequent calls to otaHandler.update() from loop().
// Keep global constructors lightweight; hardware initialization should happen in setup().

//Pin numbers
constexpr uint8_t SENSOR_PIN_LEFT = 0;
constexpr uint8_t SENSOR_PIN_MIDDLE = 0;
constexpr uint8_t SENSOR_PIN_RIGHT = 0;

OTAHandler otaHandler;

//each sensor is a separate object, test interval and sampling values
OpticalSensor leftSensor(SENSOR_PIN_LEFT);
OpticalSensor middleSensor(SENSOR_PIN_MIDDLE);
OpticalSensor rightSensor(SENSOR_PIN_RIGHT);



void setup() {
    Serial.begin(115200);

    const u_int32_t TIMEOUTMS = 10000;

    delay(50); //Short startup delay for WiFi/CPU stabilization
    
    //TODO: Move WiFi credentials to a local config file
    otaHandler.begin(
    "", //ssid
    "", //password 
    "",  //hostname
    TIMEOUTMS
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