#include <Arduino.h>

#include "Sensors.h"


// keep it light, do not add complex functionality because of hardwere limts

opticalSensor::opticalSensor(uint8_t analogPin,
                             uint32_t readIntervalMs = 50,
                             uint32_t sampling = 5)
{
    analog_pin = analogPin;
    readInterval = readIntervalMs;
    m_sampling = sampling;
};

void opticalSensor::begin(){
    pinMode(analog_pin, INPUT);
};

void opticalSensor::update(){
    uint32_t currentTime = millis();

    if (currentTime - lastReadTime < readInterval)
    {
        return;
    }

    lastReadTime = currentTime;

    distanceCm = convertToDistanceCm(filteredRead());
    
}

float opticalSensor::getDistanceCm(){
    return distanceCm;
}

u_int16_t opticalSensor::filteredRead(){
        uint32_t sum = 0;

        for (uint8_t i = 0; i < m_sampling; i++){
            sum += analogRead(analog_pin);
        }

        return sum / m_sampling;
}

uint16_t opticalSensor::convertToDistanceCm(uint16_t adcReadout){
    if(adcReadout < 1){ //prevent zero divison error
            return 999.0f; //outside of sensor capableity, error val, change to NAN
        }
    float distance =
    4800.0f / (adcReadout - 20); //approxmiation, check for accuracy!
    
    return distance;
}