#ifndef GUNCONTROLLER_H
#define GUNCONTROLLER_H

#include <arduino-timer.h>

class GunController {

    private:
        static const int m_delayAirsoftMustBeComplete = 5000;           // Seconds when airsoft shoot must be complete
        volatile static bool m_isFiring;                                // To know if gun is firing, i.e., running airsoft motor
        volatile static bool m_isFired;
        static byte m_airsoftCompleteInterruptPin;
        static Timer<> * m_APTimer;
        static uintptr_t m_taskAirsoftInterrupt;

        static void startAirsoftMotor();
        static bool stopAirsoftMotor(void *);
        static void enableAirsoftInterrupt();
        static void disableAirsoftInterrupt();
        static void airsoftISR(void);

    public:
        GunController();
        static void begin(Timer<> * APTimer, byte AirsoftCompleteInterruptPin);
        static bool isFiring();
        static bool isFired();
        static void readyToFire();
        static void fire();
};


#endif
