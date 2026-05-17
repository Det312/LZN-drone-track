#include <Arduino.h>

#include "Config.h"
#include "OTAHandler.h"
#include "Sensors.h"
#include "LedHandler.h"

// Avoid long blocking delays.
// OTA requires frequent calls to otaHandler.update() from loop().
// Keep global constructors lightweight; hardware initialization should happen in setup().

OTAHandler otaHandler;

//each sensor is a separate object, test interval and sampling values in config
OpticalSensor leftSensor(SENSOR_PIN_LEFT, SENSOR_READ_INTERVAL_MS, SENSOR_SAMPLING);
OpticalSensor middleSensor(SENSOR_PIN_MIDDLE, SENSOR_READ_INTERVAL_MS, SENSOR_SAMPLING);
OpticalSensor rightSensor(SENSOR_PIN_RIGHT, SENSOR_READ_INTERVAL_MS, SENSOR_SAMPLING);

//every led strip is a seperate object, change LED_COUNT according to led strip length
LedHandler innerLed(LED_PIN_INNER, LED_COUNT);
LedHandler outerLed(LED_PIN_OUTER, LED_COUNT);



void setup() {
    Serial.begin(115200);

    delay(50); //Short startup delay for WiFi/CPU stabilization
    
    otaHandler.begin(
    WIFI_SSID, //ssid
    WIFI_PASSWORD, //password 
    OTA_HOSTNAME,  //hostname
    WIFI_TIMEOUT_MS
    );

    leftSensor.begin();
    middleSensor.begin();
    rightSensor.begin();

    innerLed.begin();
    outerLed.begin();
}

void loop() {
    otaHandler.update();

    leftSensor.update();
    middleSensor.update();
    rightSensor.update();

    float leftDistance = leftSensor.getDistanceCm();
    float middleDistance = middleSensor.getDistanceCm();
    float rightDistance = rightSensor.getDistanceCm();

    if(leftDistance <= LEFT_TRGGER_DISTANCE || middleDistance <= MIDDLE_TRGGER_DISTANCE || rightDistance <= RIGHT_TRGGER_DISTANCE){
        innerLed.setAll(255, 0, 0); //RED
        outerLed.setAll(255, 0, 0); //RED
    }
    else{
        innerLed.clear();
        outerLed.clear();
    }

    innerLed.show();
    outerLed.show();

    
}