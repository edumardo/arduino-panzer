#ifndef GUNELEVATION_H
#define GUNELEVATION_H

#include <Arduino.h>
#include <AsyncServoLib.h>
#include "Radio.h"

typedef struct GunElevationProperties {
    int16_t depressionDegrees;
    int16_t elevationDegrees;
    int16_t horizontalDegrees;
    uint8_t degreesIncrement;
    int16_t mSIncrement;
};

class GunElevation {

    private:
        AsyncServo m_elevationServo;
        uint8_t m_elevationServoPin;
        int16_t  m_position;
        GunElevationProperties m_elevationProperties;
        RadioStickProperties m_radioStickProperties;
        long m_debounceMoveTime;

    public:
        GunElevation();
        void begin(uint8_t elevationServoPin, GunElevationProperties gunElevationProperties, RadioStickProperties radioStickProperties);
        void update();
        void move(int stickValue);
};

#endif
