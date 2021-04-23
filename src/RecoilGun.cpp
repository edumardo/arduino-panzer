#include "RecoilGun.h"
#include "config.h"

Timer<> * RecoilGun::m_APTimer;
AsyncServo RecoilGun::m_recoilServo;
uint16_t RecoilGun::m_mSReturn;
uint8_t RecoilGun::m_degreesIdle;

RecoilGun::RecoilGun()
    : Gun() {

}

void RecoilGun::begin(Timer<> * APTimer, uint8_t recoilServoPin, uint16_t mSRecoil, uint16_t mSReturn, uint8_t degreesIdle, uint8_t degreesRecoil) {

    m_APTimer        = APTimer;
    m_recoilServoPin = recoilServoPin;
    m_mSRecoil       = mSRecoil;
    m_mSReturn       = mSReturn;
    m_degreesIdle    = degreesIdle;
    m_degreesRecoil  = degreesRecoil;

    m_recoilServo.Attach(m_recoilServoPin);
    m_recoilServo.write(m_degreesIdle);
}

void RecoilGun::update() {
    m_recoilServo.Update();
}

void RecoilGun::fire() {

    m_readyToFire = false;
    m_recoilServo.MoveDegrees(m_degreesRecoil, m_mSRecoil, returnBarrel);
}

void RecoilGun::returnBarrel() {

    m_recoilServo.MoveDegrees(m_degreesIdle, m_mSReturn);
    m_APTimer->in(AP_GUN_RELOAD_TIME, gunReadyTofire);
}

bool RecoilGun::gunReadyTofire(void *) {

    m_readyToFire = true;
    return true;
}
