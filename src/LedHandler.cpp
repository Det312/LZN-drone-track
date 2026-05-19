#include "LedHandler.h"

#include <Arduino.h>

LedHandler::LedHandler(u_int8_t dataPin, uint16_t ledCount)
: m_strip(ledCount, dataPin, NEO_GRB + NEO_KHZ800) {  //check for RGB/GRB
}


void LedHandler::begin(){
    m_strip.begin();
    m_strip.clear();
    m_strip.show();

}

void LedHandler::setBrightness(uint8_t brightness){
    m_strip.setBrightness(brightness);
}

void LedHandler::setAll(uint8_t red, uint8_t green, uint8_t blue){
    uint32_t color = m_strip.Color(red, green, blue);
    m_strip.fill(color);
}

void LedHandler::show(){
    m_strip.show();
}

void LedHandler::clear(){
    m_strip.clear();

}