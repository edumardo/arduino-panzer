#include "GunRecoil.h"
#include "config.h"

Timer<> * GunRecoil::m_APTimer;
AsyncServo GunRecoil::m_recoilServo;
GunRecoilProperties GunRecoil::m_gunRecoilProperties;

GunRecoil::GunRecoil()
    : Gun() {

}

void GunRecoil::begin(Timer<> * APTimer, GunRecoilProperties recoilProperties) {

    m_APTimer        = APTimer;
    m_gunRecoilProperties = recoilProperties;

    m_recoilServo.Attach(m_gunRecoilProperties.servoPin);
    m_recoilServo.write(m_gunRecoilProperties.degreesIdle);
}

void GunRecoil::update() {

    m_recoilServo.Update();
}

void GunRecoil::fire() {

    m_readyToFire = false;
    m_recoilServo.MoveDegrees(m_gunRecoilProperties.degreesRecoil, m_gunRecoilProperties.mSRecoil, returnBarrel);
}

void GunRecoil::returnBarrel() {

    m_recoilServo.MoveDegrees(m_gunRecoilProperties.degreesIdle, m_gunRecoilProperties.mSReturn);
    m_APTimer->in(AP_GUN_RELOAD_TIME, gunReadyTofire);
}

bool GunRecoil::gunReadyTofire(void *) {

    m_readyToFire = true;
    return true;
}
