#include <Arduino.h>
#include <WiFi.h>

#include "Config.h"
#include "OTAHandler.h"
#include "Sensors.h"
#include "LedHandler.h"
#include "EspNowHandler.h"
#include "ComProtocol.h"

// Avoid long blocking delays.
// OTA requires frequent calls to otaHandler.update() from loop().
// Keep global constructors lightweight; hardware initialization should happen in setup().

OTAHandler otaHandler;

// Each sensor is a separate object.
// Test interval and sampling values are configured in Config.h.
OpticalSensor leftSensor(
    SENSOR_PIN_LEFT,
    SENSOR_READ_INTERVAL_MS,
    SENSOR_SAMPLING
);

OpticalSensor middleSensor(
    SENSOR_PIN_MIDDLE,
    SENSOR_READ_INTERVAL_MS,
    SENSOR_SAMPLING
);

OpticalSensor rightSensor(
    SENSOR_PIN_RIGHT,
    SENSOR_READ_INTERVAL_MS,
    SENSOR_SAMPLING
);

// Every LED strip is a separate object.
// Change LED_COUNT according to strip length.
LedHandler innerLed(
    LED_PIN_INNER,
    LED_COUNT
);

LedHandler outerLed(
    LED_PIN_OUTER,
    LED_COUNT
);

BrokerCom brokerCom;


enum class LedControlState : uint8_t {
    IdlePreset,
    DetectedPreset,
    IntermediatePreset,
    SolidColor,
    Off
};

LedControlState currentLedState =
    LedControlState::IdlePreset;

PresetCategory currentPresetCategory =
    PresetCategory::Idle;

uint8_t currentPresetIndex = 0;

uint8_t solidRed = 0;
uint8_t solidGreen = 0;
uint8_t solidBlue = 0;

GateMode currentGateMode =
    GateMode::Remote;

WiFiServer telnetServer(23);
WiFiClient telnetClient;

LedHandler::IdleAnimationMode currentIdleAnimation;
LedHandler::DetectedAnimationMode currentDetectedAnimation;

LedHandler::IdleAnimationMode randomIdleAnimation() {
    return static_cast<LedHandler::IdleAnimationMode>(
        random(0, 4)
    );
}

LedHandler::DetectedAnimationMode randomDetectedAnimation() {
    return static_cast<LedHandler::DetectedAnimationMode>(
        random(0, 4)
    );
}

LedHandler::IdleAnimationMode mapIdlePreset(uint8_t presetIndex) {
    return static_cast<LedHandler::IdleAnimationMode>(
        presetIndex % 4
    );
}

LedHandler::DetectedAnimationMode mapDetectedPreset(uint8_t presetIndex) {
    return static_cast<LedHandler::DetectedAnimationMode>(
        presetIndex % 4
    );
}

void applyBrokerCommand(const BrokerCommandMessage& command) {
    switch (command.commandType) {
        case CommandType::SetColor: {
            solidRed = command.param1;
            solidGreen = command.param2;
            solidBlue = command.param3;

            currentLedState =
                LedControlState::SolidColor;

            innerLed.setAll(
                solidRed,
                solidGreen,
                solidBlue
            );

            outerLed.setAll(
                solidRed,
                solidGreen,
                solidBlue
            );

            innerLed.show();
            outerLed.show();

            break;
        }

        case CommandType::SetPreset: {
            currentPresetCategory =
                static_cast<PresetCategory>(
                    command.param1
                );

            currentPresetIndex =
                command.param2;

            if (currentPresetCategory ==
                PresetCategory::Idle) {

                currentLedState =
                    LedControlState::IdlePreset;

            } else if (currentPresetCategory ==
                       PresetCategory::Detected) {

                currentLedState =
                    LedControlState::DetectedPreset;

            } else if (currentPresetCategory ==
                       PresetCategory::Intermediate) {

                currentLedState =
                    LedControlState::IntermediatePreset;
            }

            break;
        }

        case CommandType::SetMode: {
            GateMode requestedMode =
                static_cast<GateMode>(
                    command.param1
                );

            if (requestedMode == GateMode::Remote ||
                requestedMode == GateMode::Offline) {

                currentGateMode =
                    requestedMode;
            }

            break;
        }

        case CommandType::ClearLeds: {
            currentLedState =
                LedControlState::Off;

            innerLed.clear();
            outerLed.clear();

            innerLed.show();
            outerLed.show();

            break;
        }

        case CommandType::SetBrightness: {
            innerLed.setBrightness(command.param1);
            outerLed.setBrightness(command.param1);

            break;
        }

        case CommandType::None:
        default:
            break;
    }
}

void updateRemoteLedState() {
    switch (currentLedState) {
        case LedControlState::IdlePreset: {
            LedHandler::IdleAnimationMode mode =
                mapIdlePreset(currentPresetIndex);

            innerLed.idleAnimation(mode);
            outerLed.idleAnimation(mode);

            break;
        }

        case LedControlState::DetectedPreset: {
            LedHandler::DetectedAnimationMode mode =
                mapDetectedPreset(currentPresetIndex);

            innerLed.detectedAnimation(mode);
            outerLed.detectedAnimation(mode);

            break;
        }

        case LedControlState::IntermediatePreset: {
            // Placeholder until real intermediate presets are added.
            // For now, reuse idle presets.
            LedHandler::IdleAnimationMode mode =
                mapIdlePreset(currentPresetIndex);

            innerLed.idleAnimation(mode);
            outerLed.idleAnimation(mode);

            break;
        }

        case LedControlState::SolidColor:
            // Solid color is pushed when the command is received.
            break;

        case LedControlState::Off:
            // LEDs are cleared when the command is received.
            break;
    }
}

// TODO: Delete telnet debug when ESP-NOW communication is stable.
void telnet_debug(float left, float middle, float right) {
    if (telnetServer.hasClient()) {
        if (telnetClient && telnetClient.connected()) {
            telnetServer.available().stop();
        } else {
            telnetClient = telnetServer.available();
        }
    }

    static unsigned long lastTelnetPrint = 0;

    if (telnetClient && telnetClient.connected()) {
        if (millis() - lastTelnetPrint >= 250) {
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

    delay(50); // Short startup delay for WiFi/CPU stabilization.

    otaHandler.begin(
        WIFI_SSID,
        WIFI_PASSWORD,
        OTA_HOSTNAME,
        WIFI_TIMEOUT_MS
    );

    brokerCom.begin(
        BROKER_MAC,
        GATE_ID
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

    currentIdleAnimation =
        randomIdleAnimation();

    currentDetectedAnimation =
        randomDetectedAnimation();

    analogReadResolution(12);
    analogSetAttenuation(ADC_11db);
}

void loop() {
    otaHandler.update();

    leftSensor.update();
    middleSensor.update();
    rightSensor.update();

    uint16_t leftDistance =
        leftSensor.getDistanceCm();

    uint16_t middleDistance =
        middleSensor.getDistanceCm();

    uint16_t rightDistance =
        rightSensor.getDistanceCm();

    telnet_debug(
        leftDistance,
        middleDistance,
        rightDistance
    );

    bool objectDetected =
        leftDistance <= LEFT_TRIGGER_DISTANCE ||
        middleDistance <= MIDDLE_TRIGGER_DISTANCE ||
        rightDistance <= RIGHT_TRIGGER_DISTANCE;

    while (brokerCom.hasCommand()) {
        BrokerCommandMessage command =
            brokerCom.readCommand();

        applyBrokerCommand(command);
    }

    static bool lastObjectDetected = false;

    bool triggerEdge =
        objectDetected && !lastObjectDetected;

    lastObjectDetected =
        objectDetected;

    if (triggerEdge) {
        brokerCom.sendTrigger();
    }

    if (currentGateMode == GateMode::Remote) {
        updateRemoteLedState();
        return;
    }

    static bool offlineDetectedActive = false;
    static uint32_t offlineDetectedStartTime = 0;

    if (triggerEdge && !offlineDetectedActive) {
        offlineDetectedActive = true;

        offlineDetectedStartTime =
            millis();

        currentDetectedAnimation =
            randomDetectedAnimation();
    }

    if (offlineDetectedActive) {
        innerLed.detectedAnimation(
            currentDetectedAnimation
        );

        outerLed.detectedAnimation(
            currentDetectedAnimation
        );

        if (millis() - offlineDetectedStartTime >= 5000) {
            offlineDetectedActive = false;

            currentIdleAnimation =
                randomIdleAnimation();
        }

    } else {
        innerLed.idleAnimation(
            currentIdleAnimation
        );

        outerLed.idleAnimation(
            currentIdleAnimation
        );
    }
}