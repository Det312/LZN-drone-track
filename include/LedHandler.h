# pragma once 

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class LedHandler{
public:
    LedHandler(uint8_t dataPin, uint16_t ledCount);

    void begin();

    void setBrightness(uint8_t setBrightness);

    void setAll(uint8_t red, uint8_t green, uint8_t blue);
    
    void show();

    void clear();
private:
    Adafruit_NeoPixel m_strip;
};