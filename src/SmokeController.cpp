#include "SmokeController.h"
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
void SmokeController::begin(DCMotorControllerConfig smokerGeneratorConfig, DCMotorControllerConfig smokerFanConfig , SmokeGeneratorBehaviour behaviour) {

    m_behaviour = behaviour;
    m_pwmGeneratorPin = smokerGeneratorConfig.pwmPin;
    m_in1GeneratorPin = smokerGeneratorConfig.in1Pin;
    m_in2GeneratorPin = smokerGeneratorConfig.in2Pin;
    m_standbyPin = smokerGeneratorConfig.standbyPin;
    m_pwmFanPin = smokerFanConfig.pwmPin;
    m_in1FanPin = smokerFanConfig.in1Pin;
    m_in2FanPin = smokerFanConfig.in2Pin;
    pinMode(m_pwmFanPin, OUTPUT);
    pinMode(m_in1GeneratorPin, OUTPUT);
    pinMode(m_in2GeneratorPin, OUTPUT);
    pinMode(m_standbyPin, OUTPUT);
    pinMode(m_pwmFanPin, OUTPUT);
    pinMode(m_in1FanPin, OUTPUT);
    pinMode(m_in2FanPin, OUTPUT);
    disable();
}

/**
 *
 */
void SmokeController::start() {

    m_isStarted = true;
}
/**
 * 
 */
void SmokeController::idle() {

    if (m_isStarted) {
        setGeneratorSpeed(m_idlePwmFixedGenerator);
        setFanSpeed(m_idlePwmFixedFan);

        if (m_debugMode) printDebug(m_idlePwmFixedGenerator, m_idlePwmFixedFan);
    }
}

/**
 * 
 */
void SmokeController::smoke(byte speedX, byte speedY) {

    if (m_isStarted)
        (m_behaviour == fixed) ? fixedSmoke() : proportionalSmoke(speedX, speedY);
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
void SmokeController::proportionalSmoke(byte speedX, byte speedY) {

    byte absX = (speedX < m_minProportional) ? m_minProportional - speedX  + m_minProportional - 1 : speedX;
    byte absY = (speedY < m_minProportional) ? m_minProportional - speedY  + m_minProportional - 1 : speedY;
    byte absSpeed = max(absX, absY);

    byte proportionalPwmGenerartor = map(absSpeed, m_minProportional, m_maxProportional, m_idlePwmFixedGenerator, m_movingPwmFixedGenerator);
    byte proportionalPwmFan = map(absSpeed, m_minProportional, m_maxProportional, m_idlePwmFixedFan, m_movingPwmFixedFan);
    setGeneratorSpeed(proportionalPwmGenerartor);
    setFanSpeed(proportionalPwmFan);

    if (m_debugMode) printDebug(proportionalPwmGenerartor, proportionalPwmFan);
}

/**
 * 
 */
void SmokeController::setGeneratorSpeed(byte pwmGenerator) {

    digitalWrite(m_in1GeneratorPin, HIGH);
    digitalWrite(m_in2GeneratorPin, LOW);
    digitalWrite(m_standbyPin, HIGH);
    analogWrite(m_pwmGeneratorPin, pwmGenerator);
}

/**
 * 
 */
void SmokeController::setFanSpeed(byte pwmFan) {

    digitalWrite(m_in1FanPin, HIGH);
    digitalWrite(m_in2FanPin, LOW);
    digitalWrite(m_standbyPin, HIGH);
    analogWrite(m_pwmFanPin, pwmFan);
}

/**
 * 
 */
void SmokeController::disable() {

    digitalWrite(m_in1GeneratorPin, LOW);
    digitalWrite(m_in2GeneratorPin, LOW);
    analogWrite(m_pwmGeneratorPin, 0);
    digitalWrite(m_in1FanPin, LOW);
    digitalWrite(m_in2FanPin, LOW);
    analogWrite(m_pwmFanPin, 0);
    digitalWrite(m_standbyPin, LOW);
}

/**
 * 
 */
void SmokeController::printDebug(byte pwmGenerator, byte pwmFan) {

    Serial.print("[SmokeController ");
    Serial.print(m_behaviour == 0 ? "proportional" : "fixed");
    Serial.print("][Generator pwm: ");
    Serial.print(pwmGenerator);
    Serial.print("][Fan pwm: ");
    Serial.print(pwmFan);
    Serial.println("]");
}
