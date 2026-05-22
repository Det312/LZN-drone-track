# pragma once 

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class LedHandler{
public:

    enum IdleAnimationMode {
        IDLE_BREATH = 0,
        IDLE_SOFT_SCAN = 1,
        IDLE_SPARKLE = 2
    };

    LedHandler(uint8_t dataPin, uint16_t ledCount);

    //Initialize led string
    void begin();

    //Change brigtness
    void setBrightness(uint8_t brightness);

    //Change color for the whole strip
    //Mainly for debug and testing
    //TODO:Implement better LED control, move this func to private 
    void setAll(uint8_t red, uint8_t green, uint8_t blue);

    void setPixel(uint16_t index, uint8_t red, uint8_t green, uint8_t blue);
    
    //Update leds
    void show();

    //Clear led strip
    void clear();

    void idleAnimation(IdleAnimationMode mode);
    void detectedAnimation();
    
private:
    Adafruit_NeoPixel m_strip;

    uint32_t m_lastAnimUpdated;
    uint16_t m_animStep;
    bool m_animDirection;

    uint8_t m_targetRed = 0;
    uint8_t m_targetGreen = 0;
    uint8_t m_targetBlue = 255;

    uint32_t makeColor(uint8_t red, uint8_t green, uint8_t blue);
};