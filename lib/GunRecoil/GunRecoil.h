#ifndef GUNRECOIL_H
#define GUNRECOIL_H

#include <Arduino.h>
#include <AsyncServoLib.h>
#include <arduino-timer.h>
#include "Gun.h"
#include "GunRecoilProperties.h"

class GunRecoil : public Gun {

    private:
        static Timer<> * m_APTimer;
        static AsyncServo m_recoilServo;
        static GunRecoilProperties m_gunRecoilProperties;

        static void returnBarrel();
        static bool gunReadyTofire(void *);

    public:
        GunRecoil();
        void begin(Timer<> * APTimer, GunRecoilProperties recoilProperties);
        void update();
        void fire();
        void printDebug();

};

#endif
