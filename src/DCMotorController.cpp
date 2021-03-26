#include "DCMotorController.h"
#include "Utilities.h"

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
void DCMotorController::begin(DCMotorControllerConfig motorConfig, int stickProperties[3]) {

    m_pwmPrescaler = motorConfig.pwmPrescaler;
    m_pwmPin = motorConfig.pwmPin;
    m_in1Pin = motorConfig.in1Pin;
    m_in2Pin = motorConfig.in2Pin;
    m_standbyPin = motorConfig.standbyPin;

    m_stickMinValue = stickProperties[0];
    m_stickMaxValue = stickProperties[1];
    m_stickCenterValue = stickProperties[2];

    pinMode(m_pwmPin, OUTPUT);
    pinMode(m_in1Pin, OUTPUT);
    pinMode(m_in2Pin, OUTPUT);
    pinMode(m_standbyPin, OUTPUT);
    disableMotor();
    setPWMPrescaler(m_pwmPin, m_pwmPrescaler);
}

/**
 * Sets the maximum motor voltage, in percent, of the power provided.
 *
 * For example, If your battery provides 16V, yo can use this method to achieve a maximum output of 8V, passing 50 as parameter.
 * This is useful when your battery provides 16V but your motor should not run at more than 8V.
 *
 * @param percent: (0,100] value.
 */
void DCMotorController::setMaxVoltagePercent(uint8_t percent) {

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

    digitalWrite(m_pwmPin, 0);
    digitalWrite(m_in1Pin, LOW);
    digitalWrite(m_in2Pin, LOW);
    digitalWrite(m_standbyPin, LOW);
}

/**
 *
 */
void DCMotorController::move(int stickValue) {

    if (stickValue == m_stickCenterValue) return;

    digitalWrite(m_in1Pin, (stickValue < m_stickCenterValue) ? LOW : HIGH);
    digitalWrite(m_in2Pin, (stickValue < m_stickCenterValue) ? HIGH : LOW);
    int pwmSpeed = abs(map(stickValue, m_stickMinValue, m_stickMaxValue, -255, 255));
    int pwmSpeedPercent = pwmSpeed * m_maxVoltagePercent / 100;
    enableMotor();
    analogWrite(m_pwmPin, pwmSpeedPercent);

    if (m_debugMode) {

        printDEC("stickValue", stickValue);
        Serial.print("[");
        Serial.print(m_debugName);
        if (stickValue < m_stickCenterValue)
            Serial.print(" left");
        else
            Serial.print(" right");
        Serial.print("]");
        printDEC("pwmSpeedPercent", pwmSpeedPercent);
        Serial.println();
    }
}

/**
 *
 */
void DCMotorController::printDEC(String label, int value) {
    Serial.print("[");
    Serial.print(label);
    Serial.print(": ");
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
