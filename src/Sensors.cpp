#include <Arduino.h>

#include "Sensors.h"


OpticalSensor::OpticalSensor(uint8_t analogPin,
                             uint32_t readIntervalMs,
                             uint32_t sampling)
{
    m_analogPin = analogPin;
    m_readInterval = readIntervalMs;
    m_sampling = sampling;

    m_lastReadTime = 0;
    m_rawAnalog = 0;
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

float OpticalSensor::convertToDistanceCm(uint16_t adcReadout){
    float voltage = filteredRead() * (3.3 / 4095);

    if (voltage < 0.1)
        voltage = 0.2;
    
    float distance = 27.89 / (voltage - 0.1);
    
    return (distance);
}