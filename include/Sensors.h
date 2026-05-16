#pragma once

#include <Arduino.h>

// GP2Y0A21YK0F specific class, supports filtered reading to compensate for adc error.
class OpticalSensor {
public:
    OpticalSensor(uint8_t analogPin,
                  uint32_t readIntervalMs = 50,
                  uint32_t sampling = 5);

    //Initialize the sensor              
    void begin();
    
    //Update every loop() for correct functioning
    void update();

    //Returns the distance in Cm
    float getDistanceCm(); 



private:
    uint8_t m_analogPin;
    uint32_t m_readInterval; //time beeteween measurements
    uint32_t m_sampling;     //number of samples in a single distance check, reference the filteredRead() function.
    uint32_t m_lastReadTime;
    uint32_t m_rawAnalog;
    
    float m_distanceCm;


    //Returns the average of analogs read specified by sampling and readInterval. Too many samples may cause problems with CPU time usege
    uint16_t filteredRead();

    //Converts the adc readout to distance in cm. Highly recommended to not use raw adc values due to errors.
    uint16_t convertToDistanceCm(uint16_t adcReadout);
    
};