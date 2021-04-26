#include "GunRecoil.h"
#include "config.h"

Timer<> * GunRecoil::m_APTimer;
AsyncServo GunRecoil::m_recoilServo;
uint16_t GunRecoil::m_mSReturn;
uint8_t GunRecoil::m_degreesIdle;

GunRecoil::GunRecoil()
    : Gun() {

}

void GunRecoil::begin(Timer<> * APTimer, uint8_t recoilServoPin, uint16_t mSRecoil, uint16_t mSReturn, uint8_t degreesIdle, uint8_t degreesRecoil) {

    m_APTimer        = APTimer;
    m_recoilServoPin = recoilServoPin;
    m_mSRecoil       = mSRecoil;
    m_mSReturn       = mSReturn;
    m_degreesIdle    = degreesIdle;
    m_degreesRecoil  = degreesRecoil;

    m_recoilServo.Attach(m_recoilServoPin);
    m_recoilServo.write(m_degreesIdle);
}

void GunRecoil::update() {
    m_recoilServo.Update();
}

void GunRecoil::fire() {

    m_readyToFire = false;
    m_recoilServo.MoveDegrees(m_degreesRecoil, m_mSRecoil, returnBarrel);
}

void GunRecoil::returnBarrel() {

    m_recoilServo.MoveDegrees(m_degreesIdle, m_mSReturn);
    m_APTimer->in(AP_GUN_RELOAD_TIME, gunReadyTofire);
}

bool GunRecoil::gunReadyTofire(void *) {

    m_readyToFire = true;
    return true;
}
