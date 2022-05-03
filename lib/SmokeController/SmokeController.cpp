#include "SmokeController.h"
#include "Utilities.h"

/**
 *
 */
SmokeController::SmokeController() {

    m_debugMode = false;
    m_isStarted = false;
    m_behaviour = SmokeGeneratorBehaviour::fixed;
    m_maxGeneratorVoltagePercent = m_maxFanVoltagePercent = 100;
    m_idleGeneratorVoltagePercent = m_idleFanVoltagePercent = 50;
    m_movingGeneratorVoltagePercent = m_movingFanVoltagePercent = 100;
    m_idlePwmFixedGenerator = m_idlePwmFixedFan = (((255 * m_maxGeneratorVoltagePercent) / 100) * m_idleGeneratorVoltagePercent) / 100;
    m_movingPwmFixedGenerator = m_movingPwmFixedFan = (((255 * m_maxGeneratorVoltagePercent) / 100) * m_movingGeneratorVoltagePercent) / 100;
}

/**
 *
 */
void SmokeController::begin(DCMotorControllerProperties smokerGeneratorProperties, DCMotorControllerProperties smokerFanProperties, SmokeGeneratorBehaviour behaviour) {

    m_behaviour = behaviour;
    m_smokerGeneratorProperties = smokerGeneratorProperties;
    m_smokerFanProperties = smokerFanProperties;

    pinMode(m_smokerGeneratorProperties.pwmPin, OUTPUT);
    pinMode(m_smokerGeneratorProperties.in1Pin, OUTPUT);
    pinMode(m_smokerGeneratorProperties.in2Pin, OUTPUT);
    pinMode(m_smokerGeneratorProperties.standbyPin, OUTPUT);
    pinMode(m_smokerFanProperties.pwmPin, OUTPUT);
    pinMode(m_smokerFanProperties.in1Pin, OUTPUT);
    pinMode(m_smokerFanProperties.in2Pin, OUTPUT);
    disable();
    setPWMPrescaler(m_smokerGeneratorProperties.pwmPin, m_smokerGeneratorProperties.pwmPrescaler);
    setPWMPrescaler(m_smokerFanProperties.pwmPin, m_smokerFanProperties.pwmPrescaler);
}

/**
 *
 */
void SmokeController::toggle() {

    if (m_isStarted) disable();

    m_isStarted = !m_isStarted;

    if (m_debugMode) {
        Serial.print("[SmokeController from ");
        Serial.print(!m_isStarted ? "on" : "off");
        Serial.println(!m_isStarted ? " to off]" : " to on]");
    }
}

/**
 *
 */
void SmokeController::idle() {

    if (m_isStarted) {
        setGeneratorSpeed(m_idlePwmFixedGenerator);
        setFanSpeed(m_idlePwmFixedFan);
    }

    if (m_debugMode) printDebug(m_idlePwmFixedGenerator, m_idlePwmFixedFan);

}

/**
 *
 */
void SmokeController::smoke(byte speed) {

    if (m_isStarted)
        (m_behaviour == fixed) ? fixedSmoke() : proportionalSmoke(speed);
}

/**
 *
 */
void SmokeController::setProportional(byte minProportional, byte maxProportional) {

    m_minProportional = minProportional;
    m_maxProportional = maxProportional;
}

/**
 *
 */
void SmokeController::setGeneratorVoltagesPercent(byte maxGeneratorVoltagePercent, byte idleGeneratorVoltagePercent, byte movinGeneratorVoltagePercent) {

    m_maxGeneratorVoltagePercent = maxGeneratorVoltagePercent;
    m_idleGeneratorVoltagePercent = idleGeneratorVoltagePercent;
    m_movingGeneratorVoltagePercent = movinGeneratorVoltagePercent;
    m_idlePwmFixedGenerator = (((255 * m_maxGeneratorVoltagePercent) / 100) * m_idleGeneratorVoltagePercent) / 100;
    m_movingPwmFixedGenerator = (((255 * m_maxGeneratorVoltagePercent) / 100) * m_movingGeneratorVoltagePercent) / 100;
}

/**
 *
 */
void SmokeController::setFanVoltagesPercent(byte maxFanVoltagePercent, byte idleFanVoltagePercent, byte movinFanVoltagePercent) {

    m_maxFanVoltagePercent = maxFanVoltagePercent;
    m_idleFanVoltagePercent = idleFanVoltagePercent;
    m_movingFanVoltagePercent = movinFanVoltagePercent;
    m_idlePwmFixedFan = (((255 * m_maxFanVoltagePercent) / 100) * m_idleFanVoltagePercent) / 100;
    m_movingPwmFixedFan = (((255 * m_maxFanVoltagePercent) / 100) * m_movingFanVoltagePercent) / 100;
}

/**
 *
 */
void SmokeController::enableDebug() {

    m_debugMode = true;
}

/**
 *
 */
void SmokeController::disableDebug() {

    m_debugMode = false;
}

/**
 *
 */
void SmokeController::fixedSmoke() {

    setGeneratorSpeed(m_movingPwmFixedGenerator);
    setFanSpeed(m_movingPwmFixedFan);

    if (m_debugMode) printDebug(m_movingPwmFixedGenerator, m_movingPwmFixedFan);
}

/**
 *
 */
void SmokeController::proportionalSmoke(byte speed) {

    byte proportionalPwmGenerartor = map(speed, m_minProportional, m_maxProportional, m_idlePwmFixedGenerator, m_movingPwmFixedGenerator);
    byte proportionalPwmFan = map(speed, m_minProportional, m_maxProportional, m_idlePwmFixedFan, m_movingPwmFixedFan);
    setGeneratorSpeed(proportionalPwmGenerartor);
    setFanSpeed(proportionalPwmFan);

    if (m_debugMode) printDebug(proportionalPwmGenerartor, proportionalPwmFan);
}

/**
 *
 */
void SmokeController::setGeneratorSpeed(byte pwmGenerator) {

    digitalWrite(m_smokerGeneratorProperties.in1Pin, HIGH);
    digitalWrite(m_smokerGeneratorProperties.in2Pin, LOW);
    digitalWrite(m_smokerGeneratorProperties.standbyPin, HIGH);
    analogWrite(m_smokerGeneratorProperties.pwmPin, pwmGenerator);
}

/**
 *
 */
void SmokeController::setFanSpeed(byte pwmFan) {

    digitalWrite(m_smokerFanProperties.in1Pin, HIGH);
    digitalWrite(m_smokerFanProperties.in2Pin, LOW);
    digitalWrite(m_smokerFanProperties.standbyPin, HIGH);
    analogWrite(m_smokerFanProperties.pwmPin, pwmFan);
}

/**
 *
 */
void SmokeController::disable() {

    digitalWrite(m_smokerGeneratorProperties.in1Pin, LOW);
    digitalWrite(m_smokerGeneratorProperties.in2Pin, LOW);
    analogWrite(m_smokerGeneratorProperties.pwmPin, 0);
    digitalWrite(m_smokerFanProperties.in1Pin, LOW);
    digitalWrite(m_smokerFanProperties.in2Pin, LOW);
    analogWrite(m_smokerFanProperties.pwmPin, 0);
    digitalWrite(m_smokerGeneratorProperties.standbyPin, LOW);
}

/**
 *
 */
void SmokeController::printDebug(byte pwmGenerator, byte pwmFan) {

    Serial.print("[SmokeController");
    Serial.print(m_isStarted ? " on " : " off ");
    Serial.print(m_behaviour == 0 ? "proportional" : "fixed");
    Serial.print("][Generator pwm: ");
    Serial.print(pwmGenerator);
    Serial.print("][Fan pwm: ");
    Serial.print(pwmFan);
    Serial.println("]");
}
