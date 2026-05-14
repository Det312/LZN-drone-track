#include <Arduino.h>

#include "Sensors.h"


OpticalSensor::OpticalSensor(uint8_t analogPin,
                             uint32_t readIntervalMs = 50,
                             uint32_t sampling = 5)
{
    m_analogPin = analogPin;
    m_readInterval = readIntervalMs;
    m_sampling = sampling;
};

void OpticalSensor::begin(){
    pinMode(m_analogPin, INPUT);
};

void OpticalSensor::update(){
    uint32_t currentTime = millis();

    if (currentTime - m_lastReadTime < m_readInterval)
    {
        return;
    }

    m_lastReadTime = currentTime;

    m_distanceCm = convertToDistanceCm(filteredRead());
    
}

float OpticalSensor::getDistanceCm(){
    return m_distanceCm;
}

u_int16_t OpticalSensor::filteredRead(){
        uint32_t sum = 0;

        for (uint8_t i = 0; i < m_sampling; i++){
            sum += analogRead(m_analogPin);
        }

        return sum / m_sampling;
}

uint16_t OpticalSensor::convertToDistanceCm(uint16_t adcReadout){
    if(adcReadout < 1){    //prevent zero divison error
            return 999.0f; //Error value TODO: change to NAN and add error handling
        }
    float distance =
    4800.0f / (adcReadout - 20); //approxmiation, check for accuracy
    
    return distance;
}