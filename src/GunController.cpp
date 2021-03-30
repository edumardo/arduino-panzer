#include "GunController.h"

bool GunController::m_isFiring;
bool GunController::m_isFired;
byte GunController::m_airsoftCompleteInterruptPin;
Timer<> * GunController::m_APTimer;
uintptr_t GunController::m_taskAirsoftInterrupt;

GunController::GunController() {

    m_isFiring = false;
    m_isFired = false;
}

void GunController::begin(Timer<> * APTimer, byte airsoftCompleteInterruptPin) {

    m_APTimer = APTimer;
    m_airsoftCompleteInterruptPin = airsoftCompleteInterruptPin;
    pinMode(m_airsoftCompleteInterruptPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(m_airsoftCompleteInterruptPin), airsoftISR, CHANGE);
}


bool GunController::isFiring() {

    return m_isFiring;
}

bool GunController::isFired() {

    return m_isFired;
}

void GunController::readyToFire() {
    m_isFired = false;
}

void GunController::fire() {

    startAirsoftMotor();
    enableAirsoftInterrupt();
    m_taskAirsoftInterrupt = m_APTimer->in(5000, stopAirsoftMotor);
    m_isFiring = true;
}

void GunController::startAirsoftMotor() {

    Serial.print("START startAirsoftMotor: ");
    Serial.println(millis());
    digitalWrite(40, HIGH);
}

bool GunController::stopAirsoftMotor(void *) {

    Serial.print("STOP stopAirsoftMotor: ");
    Serial.println(millis());
    Serial.println();

    digitalWrite(40, LOW);
    m_APTimer->cancel(m_taskAirsoftInterrupt);

    m_isFiring = false;
    m_isFired = true;

    return true;
}
void GunController::enableAirsoftInterrupt() {

    //Serial.println("enableAirsoftInterrupt");
    // activar pin de interrupción que lanza airsoftISR
}
void GunController::disableAirsoftInterrupt() {

    //Serial.println("disableAirsoftInterrupt");
    // desactivar pin de interrupción que lanza airsoftISR
}

void GunController::airsoftISR(void) {

    disableAirsoftInterrupt();
    stopAirsoftMotor((void *) 123);     // Random parameter
    m_isFiring = false;
    m_isFired = true;
}

void GunController::postFireActions() {

    // si el tanque está parado, retroceso

    // sonido de disparo

    // humo de disparo

    // luces de disparo
}