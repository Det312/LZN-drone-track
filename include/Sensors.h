#pragma once

#include <Arduino.h>

// GP2Y0A21YK0F specific
class opticalSensor {
public:

    opticalSensor(uint8_t analogPin,
                  uint32_t readIntervalMs = 50);

    void begin();
    
    void update();

    float getDistanceCm(); 



private:
    uint32_t sampling; //number of samples in a single measurment, to increse accuracy
    uint8_t analog_pin;
};