#include "GunController.h"

volatile bool GunController::m_isFiring;
volatile bool GunController::m_isFired;
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
    m_taskAirsoftInterrupt = m_APTimer->in(5000, stopAirsoftMotor);
    m_isFiring = true;
    enableAirsoftInterrupt();
}

void GunController::startAirsoftMotor() {

    Serial.print("START startAirsoftMotor: ");
    Serial.println(millis());
}

bool GunController::stopAirsoftMotor(void *) {

    Serial.print("STOP stopAirsoftMotor: ");
    Serial.println(millis());
    Serial.println();

    m_APTimer->cancel(m_taskAirsoftInterrupt);
    m_isFiring = false;
    m_isFired = true;

    return true;
}
void GunController::enableAirsoftInterrupt() {

    attachInterrupt(digitalPinToInterrupt(m_airsoftCompleteInterruptPin), airsoftISR, CHANGE);
}
void GunController::disableAirsoftInterrupt() {

    detachInterrupt(digitalPinToInterrupt(m_airsoftCompleteInterruptPin));
}

void GunController::airsoftISR(void) {

    disableAirsoftInterrupt();
    stopAirsoftMotor((void *) 123);     // Random parameter
    m_isFiring = false;
    m_isFired = true;
}
