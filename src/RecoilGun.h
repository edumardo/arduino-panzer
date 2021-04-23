#ifndef RECOILGUN_H
#define RECOILGUN_H

#include <Arduino.h>
#include <AsyncServoLib.h>
#include <arduino-timer.h>
#include "Gun.h"

class RecoilGun : public Gun {

    private:
        static Timer<> * m_APTimer;
        static AsyncServo m_recoilServo;
        uint8_t m_recoilServoPin;
        uint16_t m_mSRecoil;                        // miliseconds to recoil 
        static uint16_t m_mSReturn;                 // miliseconds to return
        static uint8_t m_degreesIdle;               // Idle degrees of the servo
        uint8_t m_degreesRecoil;                    // Maximum recoil degrees of the servo

        static void returnBarrel();
        static bool gunReadyTofire(void *);

    public:
        RecoilGun();
        void begin(Timer<> * APTimer, uint8_t recoilServoPin, uint16_t mSRecoil, uint16_t mSReturn, uint8_t degreesIdle, uint8_t degreesRecoil);
        void update();
        void fire();
        void printDebug();

};

#endif
