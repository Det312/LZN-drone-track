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

void LedHandler::setPixel(uint16_t index,
                          uint8_t red,
                          uint8_t green,
                          uint8_t blue){
    if (index >= m_strip.numPixels()) {
        return;
    }

    m_strip.setPixelColor(index, m_strip.Color(red, green, blue));
}

void LedHandler::show(){
    m_strip.show();
}

void LedHandler::clear(){
    m_strip.clear();
}

uint32_t LedHandler::makeColor(uint8_t red, uint8_t green, uint8_t blue) {
    return m_strip.Color(red, green, blue);
}


void LedHandler::idleAnimation(IdleAnimationMode mode) {
    uint32_t now = millis();

    uint32_t intervalMs = 40;

    if (mode == IDLE_BREATH) {
        intervalMs = 20;   // szybciej
    }

    if (mode == IDLE_SOFT_SCAN) {
        intervalMs = 15;   // szybciej
    }

    if (now - m_lastAnimUpdated < intervalMs) {
        return;
    }

    m_lastAnimUpdated = now;

    m_strip.setBrightness(35);

    switch (mode) {

        case IDLE_BREATH: {
            if (m_animDirection) {

        m_animStep += 5;

        if (m_animStep >= 120) {
            m_animStep = 120;
            m_animDirection = false;
        }

    } else {

        if (m_animStep > 2) {
            m_animStep -= 2;

        } else {

            m_animStep = 5;
            m_animDirection = true;

            // Losuj nowy kolor
            m_targetRed = random(20, 120);
            m_targetGreen = random(20, 120);
            m_targetBlue = random(20, 120);

            // Unikaj zbyt ciemnych kolorów
            if (m_targetRed < 40 &&
                m_targetGreen < 40 &&
                m_targetBlue < 40) {

                m_targetBlue = 100;
            }
        }
    }

    float brightness =
        static_cast<float>(m_animStep) / 120.0f;

    uint8_t red =
        static_cast<uint8_t>(
            m_targetRed * brightness
        );

    uint8_t green =
        static_cast<uint8_t>(
            m_targetGreen * brightness
        );

    uint8_t blue =
        static_cast<uint8_t>(
            m_targetBlue * brightness
        );

    m_strip.fill(
        makeColor(red, green, blue)
    );

    break;
            
        }

        case IDLE_SOFT_SCAN: {
           m_strip.clear();

    uint16_t ledCount =
        m_strip.numPixels();

    uint16_t center1 =
        m_animStep % ledCount;

    uint16_t center2 =
        (center1 + ledCount / 2)
        % ledCount;

    const int8_t tailSize = 10;

    // Pierwszy punkt
    for (int8_t offset = -tailSize;
         offset <= tailSize;
         offset++) {

        int16_t index =
            center1 + offset;

        if (index < 0) {
            index += ledCount;
        }

        if (index >= ledCount) {
            index -= ledCount;
        }

        uint8_t intensity =
            120 - abs(offset) * 10;

        m_strip.setPixelColor(
            index,
            makeColor(
                0,
                intensity,
                intensity
            )
        );
    }

    // Drugi punkt
    for (int8_t offset = -tailSize;
         offset <= tailSize;
         offset++) {

        int16_t index =
            center2 + offset;

        if (index < 0) {
            index += ledCount;
        }

        if (index >= ledCount) {
            index -= ledCount;
        }

        uint8_t intensity =
            120 - abs(offset) * 10;

        m_strip.setPixelColor(
            index,
            makeColor(
                intensity,
                0,
                intensity
            )
        );
    }

    m_animStep += 2;

    break;
    }

        case IDLE_SPARKLE: {
            m_strip.fill(makeColor(0, 0, 8));

            for (uint8_t i = 0; i < 6; i++) {
                uint16_t index = random(m_strip.numPixels());
                m_strip.setPixelColor(index, makeColor(0, 20, 35));
            }

            break;
        }
    }

    m_strip.show();
}

void LedHandler::detectedAnimation() {
    uint32_t now = millis();

    if (now - m_lastAnimUpdated < 25) {
        return;
    }

    m_lastAnimUpdated = now;

    m_strip.setBrightness(255);

    uint8_t red = 255;
    uint8_t green = 0;
    uint8_t blue = 0;

    for (uint16_t i = 0; i < m_strip.numPixels(); i++) {
        uint16_t wave = (i + m_animStep) % 30;

        if (wave < 10) {
            m_strip.setPixelColor(i, makeColor(red, green, blue));
        } else {
            m_strip.setPixelColor(i, makeColor(80, 0, 0));
        }
    }

    m_animStep++;
    m_strip.show();
}