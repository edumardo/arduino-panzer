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
    m_debugMode = false;
}

/**
 *
 */
void DriveDirection::begin(DCMotorControllerConfig leftMotorConfig, DCMotorControllerConfig rightMotorConfig, int stickProperties[3]){

    m_stickMinValue = stickProperties[0];
    m_stickMaxValue = stickProperties[1];
    
    int leftMotorStickProperties[3] = {m_invertYStick ? m_diffSteerComputeRange : -m_diffSteerComputeRange, m_invertYStick ? -m_diffSteerComputeRange : m_diffSteerComputeRange, 0};
    m_leftMotor.begin(leftMotorConfig, leftMotorStickProperties);
    m_leftMotor.setMaxVoltagePercent(m_analogMovePercent);
    
    int  rightMotorStickProperties[3] = {m_invertYStick ? m_diffSteerComputeRange : -m_diffSteerComputeRange, m_invertYStick ? -m_diffSteerComputeRange : m_diffSteerComputeRange, 0};
    m_rightMotor.begin(rightMotorConfig, rightMotorStickProperties);
    m_rightMotor.setMaxVoltagePercent(m_analogMovePercent);
}

/**
 *
 */
void DriveDirection::move(byte stickValueX, byte stickValueY) {

    if (m_debugMode) {
        Serial.println("[XY DriveDirection]");
    }

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

    if (m_debugMode) {
        Serial.println();
    }
}

/**
 *
 */
void DriveDirection::move(byte padUp, byte padDown, byte padLeft, byte padRight) {

    if (m_debugMode) {
        Serial.println("[UDLR DriveDirection]");
    }

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
        m_rightMotor.move(m_diffSteerComputeRange);
    }
    else if (padRight) {
        m_leftMotor.move(m_diffSteerComputeRange);
    }

    if (m_debugMode) {
        Serial.println();
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

    m_debugMode = true;
    m_leftMotor.enableDebug("left motor");
    m_rightMotor.enableDebug("right motor");
}

/**
 *
 */
void DriveDirection::disableDebug() {

    m_debugMode = false;
    m_leftMotor.disableDebug();
    m_rightMotor.disableDebug();
}
