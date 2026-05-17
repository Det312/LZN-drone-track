# pragma once 

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class LedHandler{
public:
    LedHandler(uint8_t dataPin, uint16_t ledCount);

    //Initialize led string
    void begin();

    //Change brigtness
    void setBrightness(uint8_t brightness);

    //Change color for the whole strip
    //Mainly for debug and testing
    //TODO:Implement better LED control, move this func to private 
    void setAll(uint8_t red, uint8_t green, uint8_t blue);
    
    //Update leds
    void show();

    //Clear led strip
    void clear();
    
private:
    Adafruit_NeoPixel m_strip;
};