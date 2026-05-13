#include <Arduino.h>

#include "Sensors.h"

opticalSensor::opticalSensor(){}; // empty because of hardwere limitations

void opticalSensor::begin(uint8_t GPIO, int detection_distance, int samples){

    analog_pin(GPIO);


}

void opticalSensor::checkSensor(){
    analogRead(GPIO); 
}
