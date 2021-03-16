#include "SmokeController.h"
/**
 * 
 */
SmokeController::SmokeController() {

    m_debugMode = false;
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
void SmokeController::begin(int pinSmokeGenerator[4], int pinSmokeFan[4], SmokeGeneratorBehaviour behaviour) {

    m_behaviour = behaviour;
    m_pwmGeneratorPin = pinSmokeGenerator[0];
    m_in1GeneratorPin = pinSmokeGenerator[1];
    m_in2GeneratorPin = pinSmokeGenerator[2];
    m_standbyPin = pinSmokeGenerator[3];
    m_pwmFanPin = pinSmokeFan[0];
    m_in1FanPin = pinSmokeFan[1];
    m_in2FanPin = pinSmokeFan[2];
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
void SmokeController::idle() {

    setGeneratorSpeed(m_idlePwmFixedGenerator);
    setFanSpeed(m_idlePwmFixedFan);

    if (m_debugMode) printDebug(m_idlePwmFixedGenerator, m_idlePwmFixedFan);
}

/**
 * 
 */
void SmokeController::smoke(byte speedX, byte speedY) {

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
    Serial.print(m_behaviour);
    Serial.print("][Generator pwm: ");
    Serial.print(pwmGenerator);
    Serial.print("][Fan pwm: ");
    Serial.print(pwmFan);
    Serial.println("]");
}
