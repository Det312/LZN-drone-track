#include <Arduino.h>

// DO NOT USE delay(), OTA is NOT HAPPY WITH IT


#include "OTAHandler.h"


OTAHandler otaHandler;

void setup() {

    Serial.begin(115200);

    otaHandler.begin(
        "", //ssid
        "",//password 
        ""//hostname
    );
}

void loop() {
    otaHandler.update();


}