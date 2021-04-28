#include "GunElevation.h"
#include "config.h"

GunElevation::GunElevation() {

    m_position = 0;
    m_debounceMoveTime = 0;
}

void GunElevation::begin(uint8_t elevationServoPin, GunElevationProperties gunElevationProperties, RadioStickProperties radioStickProperties) {

    m_position = m_elevationProperties.horizontalDegrees;
    m_elevationServoPin = elevationServoPin;
    m_elevationProperties = gunElevationProperties;
    m_radioStickProperties = radioStickProperties;

    m_elevationServo.Attach(m_elevationServoPin);
    m_elevationServo.WriteDegree(m_position);
}

void GunElevation::update() {

    m_elevationServo.Update();
}

void GunElevation::move(int stickValue) {

    uint8_t incDegrees = m_elevationProperties.degreesIncrement;
    int16_t maxDegrees = m_elevationProperties.horizontalDegrees + m_elevationProperties.elevationDegrees;
    int16_t minDegrees = m_elevationProperties.horizontalDegrees + m_elevationProperties.depressionDegrees;

    if (millis() - m_debounceMoveTime > m_elevationProperties.mSIncrement) {
        m_debounceMoveTime = millis();
        // Elevation
        if (stickValue < m_radioStickProperties.centerStick) {
            if ((m_position + incDegrees) <= maxDegrees) {
                m_position += incDegrees;
                m_elevationServo.MoveDegrees(m_position, m_elevationProperties.mSIncrement);
            }
        }
        // Depression
        else {
            if ((m_position - incDegrees) >= minDegrees) {
                m_position -= incDegrees;
                m_elevationServo.MoveDegrees(m_position, m_elevationProperties.mSIncrement);
            }
        }
    }
}
