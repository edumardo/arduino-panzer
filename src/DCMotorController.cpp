#include "DCMotorController.h"
#include <Arduino.h>
/**
 *
 */
DCMotorController::DCMotorController() {

    m_debugMode = false;
}

/**
 *
 */
void DCMotorController::begin(int pinMotor[4], int stickMinValue, int stickMaxValue, int stickCenterValue) {

    m_pwmPin = pinMotor[0];
    m_in1Pin = pinMotor[1];
    m_in2Pin = pinMotor[2];
    m_standbyPin = pinMotor[3];
    m_stickMinValue = stickMinValue;
    m_stickMaxValue = stickMaxValue;
    m_stickCenterValue = stickCenterValue;
    pinMode(m_pwmPin, OUTPUT);
    pinMode(m_in1Pin, OUTPUT);
    pinMode(m_in2Pin, OUTPUT);
    disableMotor();
}

/**
 *
 */
void DCMotorController::enableMotor() {

    digitalWrite(m_standbyPin, HIGH);
}

/**
 *
 */
void DCMotorController::disableMotor() {

    digitalWrite(m_standbyPin, LOW);
}

/**
 *
 */
void DCMotorController::move(int stickValue) {

    digitalWrite(m_in1Pin, (stickValue < m_stickCenterValue) ? LOW : HIGH);
    digitalWrite(m_in2Pin, (stickValue < m_stickCenterValue) ? HIGH : LOW);
    int pwmSpeed = abs(map(stickValue, m_stickMinValue, m_stickMaxValue, -255, 255));
    enableMotor();
    analogWrite(m_pwmPin, pwmSpeed);

    if (m_debugMode) {
        print(stickValue);
        if (stickValue < m_stickCenterValue) Serial.print("[Motor turn left]"); else Serial.print("[Motor turn right]");
        print(pwmSpeed);
    }
}

/**
 *
 */
void DCMotorController::print(int speed) {
    Serial.print("[");
    Serial.print(speed, DEC);
    Serial.print("]");
}

/**
 *
 */
void DCMotorController::enableDebug() {

    m_debugMode = true;
}

/**
 *
 */
void DCMotorController::disableDebug() {

    m_debugMode = false;
}
