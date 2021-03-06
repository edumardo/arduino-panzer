#include "DCMotorController.h"

/**
 *
 */
DCMotorController::DCMotorController() {

    /* Disable debug messages to serial */
    m_debugMode = false;

    /* By default, we output the PWM signal at 100 % */
    m_maxVoltagePercent = 100;

    /* Default name for debug output */
    m_debugName = "DC Motor";
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
 * Sets the maximum motor voltage, in percent, of the power provided.
 *
 * For example, If your battery provides 16V, yo can use this method to achieve a maximum output of 8V, passing 50 as parameter.
 * This is useful when your battery provides 16V but your motor should not run at more than 8V.
 *
 * @param percent: (0,100] value.
 */
void DCMotorController::setMaxVoltagePercent(int percent) {

    m_maxVoltagePercent = (percent <= 0 || percent > 100) ? 100 : percent;
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
    int pwmSpeedPercent = pwmSpeed * m_maxVoltagePercent / 100;
    enableMotor();
    analogWrite(m_pwmPin, pwmSpeedPercent);

    if (m_debugMode) {

        printDEC(stickValue);
        Serial.print("[");
        Serial.print(m_debugName);
        if (stickValue < m_stickCenterValue)
            Serial.print(" left");
        else 
            Serial.print(" right");
        Serial.print("]");
        printDEC(pwmSpeedPercent);
    }
}

/**
 *
 */
void DCMotorController::printDEC(int value) {
    Serial.print("[");
    Serial.print(value, DEC);
    Serial.print("]");
}

/**
 *
 */
void DCMotorController::enableDebug(String debugName) {

    m_debugMode = true;
    m_debugName = debugName;
}

/**
 *
 */
void DCMotorController::disableDebug() {

    m_debugMode = false;
}
