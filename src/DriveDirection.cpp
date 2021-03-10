#include "DriveDirection.h"

/**
 *
 */
DriveDirection::DriveDirection() {

    m_invertYStick = false;
    m_analogMovePercent = 100;
    m_padMovePercent = 100;
    m_diffSteer.begin(m_pivotYLimit);
    m_diffSteerComputeRange = m_diffSteer.getComputeRange();
}

/**
 *
 */
void DriveDirection::begin(int pinLeftMotor[4], int pinRightMotor[4], int stickProperties[3]){

    m_stickMinValue = stickProperties[0];
    m_stickMaxValue = stickProperties[1];
    m_leftMotor.begin(pinLeftMotor, m_invertYStick ? m_diffSteerComputeRange : -m_diffSteerComputeRange, m_invertYStick ? -m_diffSteerComputeRange : m_diffSteerComputeRange, 0);
    m_rightMotor.begin(pinRightMotor, m_invertYStick ? m_diffSteerComputeRange : -m_diffSteerComputeRange, m_invertYStick ? -m_diffSteerComputeRange : m_diffSteerComputeRange, 0);
}

/**
 *
 */
void DriveDirection::move(byte stickValueX, byte stickValueY) {

    m_diffSteer.computeMotors(
        map(stickValueX,
            m_stickMinValue,
            m_stickMaxValue,
            -m_diffSteerComputeRange,
            m_diffSteerComputeRange)
        ,
        map(stickValueY,
            m_stickMinValue,
            m_stickMaxValue,
            m_invertYStick ? m_diffSteerComputeRange : -m_diffSteerComputeRange,
            m_invertYStick ? -m_diffSteerComputeRange : m_diffSteerComputeRange)
        );

    m_leftMotor.setMaxVoltagePercent(m_analogMovePercent);
    m_rightMotor.setMaxVoltagePercent(m_analogMovePercent);
    m_leftMotor.move(m_diffSteer.computedLeftMotor());
    m_rightMotor.move(m_diffSteer.computedRightMotor());
}

/**
 *
 */
void DriveDirection::move(byte padUp, byte padDown, byte padLeft, byte padRight) {

    m_leftMotor.setMaxVoltagePercent(m_padMovePercent);
    m_rightMotor.setMaxVoltagePercent(m_padMovePercent);

    if (padUp) {
        m_leftMotor.move(m_diffSteerComputeRange);
        m_rightMotor.move(m_diffSteerComputeRange);
    }
    else if (padDown) {
        m_leftMotor.move(-m_diffSteerComputeRange);
        m_rightMotor.move(-m_diffSteerComputeRange);
    }
    else if (padLeft) {
        m_leftMotor.disableMotor();
        m_rightMotor.move(m_diffSteerComputeRange);
    }
    else if (padRight) {
        m_leftMotor.move(m_diffSteerComputeRange);
        m_rightMotor.disableMotor();
    }
}

/**
 *
 */
void DriveDirection::setMaxVoltagePercentAnalogMove(byte analogMovePercent) {

    m_analogMovePercent = analogMovePercent;
}

/**
 *
 */
void DriveDirection::setMaxVoltagePercentPadMove(byte padMovePercent) {

    m_padMovePercent = padMovePercent;
}

/**
 *
 */
void DriveDirection::invertYStick() {

    m_invertYStick = true;
}

/**
 *
 */
void DriveDirection::disableMotors() {

    m_leftMotor.disableMotor();
    m_rightMotor.disableMotor();
}

/**
 *
 */
void DriveDirection::enableDebug() {

    m_leftMotor.enableDebug("DriveDirection left motor");
    m_rightMotor.enableDebug("DriveDirection right motor");
}

/**
 *
 */
void DriveDirection::disableDebug() {

    m_leftMotor.disableDebug();
    m_rightMotor.disableDebug();
}
