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
void DCMotorController::begin(DCMotorControllerProperties motorControllerProperties, RadioStickProperties radioStickProperties) {

    m_motorProperties = motorControllerProperties;
    m_radioStickProperties = radioStickProperties;

    pinMode(m_motorProperties.pwmPin, OUTPUT);
    pinMode(m_motorProperties.in1Pin, OUTPUT);
    pinMode(m_motorProperties.in2Pin, OUTPUT);
    pinMode(m_motorProperties.standbyPin, OUTPUT);
    disableMotor();
    setPWMPrescaler(m_motorProperties.pwmPin, m_motorProperties.pwmPrescaler);
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

    digitalWrite(m_motorProperties.standbyPin, HIGH);
}

/**
 *
 */
void DCMotorController::disableMotor() {

    digitalWrite(m_motorProperties.pwmPin, 0);
    digitalWrite(m_motorProperties.in1Pin, LOW);
    digitalWrite(m_motorProperties.in2Pin, LOW);
    digitalWrite(m_motorProperties.standbyPin, LOW);
}

/**
 *
 */
void DCMotorController::move(int stickValue) {

    if (stickValue == m_radioStickProperties.centerValue) return;

    digitalWrite(m_motorProperties.in1Pin, (stickValue < m_radioStickProperties.centerValue) ? LOW : HIGH);
    digitalWrite(m_motorProperties.in2Pin, (stickValue < m_radioStickProperties.centerValue) ? HIGH : LOW);
    int pwmSpeed = abs(map(stickValue, m_radioStickProperties.minValue , m_radioStickProperties.maxValue , -255, 255));
    int pwmSpeedPercent = pwmSpeed * m_maxVoltagePercent / 100;
    enableMotor();
    analogWrite(m_motorProperties.pwmPin, pwmSpeedPercent);

    if (m_debugMode) {

        printDEC("stickValue", stickValue);
        Serial.print("[");
        Serial.print(m_debugName);
        if (stickValue < m_radioStickProperties.centerValue)
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
