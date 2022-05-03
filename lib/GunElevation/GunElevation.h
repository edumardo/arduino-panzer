#ifndef GUNELEVATION_H
#define GUNELEVATION_H

#include <Arduino.h>
#include <AsyncServoLib.h>
#include "RadioStickProperties.h"
#include "GunElevationProperties.h"

class GunElevation {

    private:
        int16_t m_position;
        unsigned long m_debounceMoveTime;
        AsyncServo m_elevationServo;
        RadioStickProperties m_radioStickProperties;
        GunElevationProperties m_elevationProperties;

    public:
        GunElevation();
        void begin(GunElevationProperties gunElevationProperties, RadioStickProperties radioStickProperties);
        void update();
        void move(int stickValue);
};

#endif
