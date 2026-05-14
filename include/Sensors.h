#pragma once

#include <Arduino.h>

// GP2Y0A21YK0F specific
class opticalSensor {
public:



    opticalSensor(uint8_t analogPin,
                  uint32_t readIntervalMs = 50,
                  uint32_t sampling = 5);

    void begin();
    
    void update();

    float getDistanceCm(); 



private:
    uint8_t analog_pin;
    uint32_t readInterval; //time beeteween measurements
    uint32_t m_sampling;     //number of samples in a single distance check, reference the filteredRead() function.
    uint32_t lastReadTime;
    uint32_t rawAnalog;
    
    float distanceCm;


    //Returns the average of analogs read specified by object properties. Too many samples may cause problems with CPU time usege
    uint16_t filteredRead();

    uint16_t convertToDistanceCm(uint16_t adcReadout);
    
};