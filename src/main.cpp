#include <Arduino.h>
#include <WiFi.h>

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

WiFiServer telnetServer(23);
WiFiClient telnetClient;

LedHandler::IdleAnimationMode currentIdleAnimation;
LedHandler::DetectedAnimationMode currentDetectedAnimation;

LedHandler::IdleAnimationMode randomIdleAnimation() {
    return static_cast<
        LedHandler::IdleAnimationMode>(
            random(0, 4)
    );
}

LedHandler::DetectedAnimationMode randomDetectedAnimation() {
    return static_cast<
        LedHandler::DetectedAnimationMode>(
            random(0, 4)
    );
}

void telnet_debug(float left, float middle, float right){ //TODO: Move to wifi manager
    if(telnetServer.hasClient()){
        if(telnetClient && telnetClient.connected()){
            telnetServer.available().stop();
        }
        else{
            telnetClient = telnetServer.available();
        }
    }

    static unsigned long lastTelnetPrint = 0;

    if( telnetClient && telnetClient.connected()){
        if(millis() - lastTelnetPrint >= 250){
            lastTelnetPrint = millis();

            telnetClient.print("L: ");
            telnetClient.print(left);
            telnetClient.print("  M: ");
            telnetClient.print(middle);
            telnetClient.print("  R: ");
            telnetClient.println(right);

        }
    }
}



void setup() {
    Serial.begin(115200);

    delay(50); //Short startup delay for WiFi/CPU stabilization
    
    otaHandler.begin(
    WIFI_SSID,
    WIFI_PASSWORD, 
    OTA_HOSTNAME, 
    WIFI_TIMEOUT_MS
    );

    telnetServer.begin();
    telnetServer.setNoDelay(true);

    leftSensor.begin();
    middleSensor.begin();
    rightSensor.begin();

    innerLed.begin();
    outerLed.begin();

    innerLed.clear();
    outerLed.clear();

    innerLed.show();
    outerLed.show();

    randomSeed(esp_random());

    currentIdleAnimation = randomIdleAnimation();

    currentDetectedAnimation = randomDetectedAnimation();

    analogReadResolution(12);
    analogSetAttenuation(ADC_11db);
}

void loop() {
    otaHandler.update();

    leftSensor.update();
    middleSensor.update();
    rightSensor.update();

    uint16_t leftDistance = leftSensor.getDistanceCm();
    uint16_t middleDistance = middleSensor.getDistanceCm();
    uint16_t rightDistance = rightSensor.getDistanceCm();

    bool objectDetected = 
            leftDistance <= LEFT_TRIGGER_DISTANCE ||
            middleDistance <= MIDDLE_TRIGGER_DISTANCE ||
            rightDistance <= RIGHT_TRIGGER_DISTANCE;

    static bool detectedModeActive = false;

    static uint32_t detectedModeStartTime = 0;

    // Start detected mode
    if (objectDetected && !detectedModeActive) {
        detectedModeActive = true;
        detectedModeStartTime = millis();
        currentDetectedAnimation = randomDetectedAnimation();
    }


    if (detectedModeActive) {
        innerLed.detectedAnimation(currentDetectedAnimation);

        outerLed.detectedAnimation(currentDetectedAnimation);

        // End detected mode
        if (millis() - detectedModeStartTime >= 3000){
            detectedModeActive = false;
            currentIdleAnimation = randomIdleAnimation();
        }
    } 
    
    else{
        innerLed.idleAnimation(currentIdleAnimation);

        outerLed.idleAnimation(currentIdleAnimation);
    }
}
