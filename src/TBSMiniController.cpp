#include "TBSMiniController.h"

/**
 *
 */
TBSMiniController::TBSMiniController() {

    m_isEngineRunning = false;
    m_debugMode = false;
    m_currentSpeed = m_PROP1_IDLE;
}

/**
 *
 */
void TBSMiniController::begin(int prop1Pin, int prop2Pin, int minSpeed, int maxSpeed) {

    m_minSpeed = minSpeed;
    m_maxSpeed = maxSpeed;
    m_prop1Pin = prop1Pin;
    m_prop2Pin = prop2Pin;

    m_prop1.attach(m_prop1Pin);
    m_prop2.attach(m_prop2Pin);
    m_prop1.writeMicroseconds(m_PROP1_IDLE);
    m_prop2.writeMicroseconds(m_PROP2_SWITCH_OFF);
}

void TBSMiniController::toggleEngine() {

    m_isEngineRunning = !m_isEngineRunning;
    m_prop2.writeMicroseconds(m_isEngineRunning ? m_PROP2_SWITCH_1 : m_PROP2_SWITCH_OFF);

    if (m_debugMode) {
        Serial.print("[TBSMiniController from ");
        Serial.print(!m_isEngineRunning ? "on" : "off");
        Serial.println(!m_isEngineRunning ? " to off]" : " to on]");
    }
}

/**
 *
 */
void TBSMiniController::idleEngine() {

    if (m_isEngineRunning) m_prop1.writeMicroseconds(m_PROP1_IDLE);

    if (m_debugMode) printDebug();

}

/**
 *
 */
void TBSMiniController::setEngineSpeed(int speed) {

    if (m_isEngineRunning) {
        m_currentSpeed = map(speed, m_minSpeed, m_maxSpeed, m_PROP1_IDLE, m_PROP1_FULL_SPEED);
        m_prop1.writeMicroseconds(m_currentSpeed);
    }

    if (m_debugMode) printDebug();
}

/**
 *
 */
bool TBSMiniController::isEngingeRunning() {

    return m_isEngineRunning;
}

/**
 *
 */
void TBSMiniController::enableDebug() {

    m_debugMode = true;
}

/**
 *
 */
void TBSMiniController::disableDebug() {

    m_debugMode = false;
}

/**
 *
 */
void TBSMiniController::printDebug() {

    Serial.print("[TBSMiniController ");
    Serial.print(m_isEngineRunning ? "on" : "off");
    Serial.print("][Speed ");
    Serial.print(m_currentSpeed);
    Serial.println("]");
}

/**
 *
 */
void TBSMiniController::playTurretRotation() {

}

/**
 *
 */
void TBSMiniController::stopTurretRotation() {

}

/**
 *
 */
void TBSMiniController::playCannon() {


}

/**
 *
 */
void TBSMiniController::stopCannon() {


}
