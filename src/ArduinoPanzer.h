#ifndef ARDUINOPANZER_H
#define ARDUINOPANZER_H

#include <Arduino.h>
#include "config.h"
#include <arduino-timer.h>
#include "PS2Radio.h"
#include "DriveDirection.h"
#include "DCMotorController.h"
#include "SmokeController.h"
#include "GunController.h"
#include <TBSMini.h>
#include <arduino-timer.h>

class ArduinoPanzer {

    private:
        PS2Radio            m_ps2x;
        DriveDirection      m_driveDirection;
        DCMotorController   m_turretRotation;
        DCMotorController   m_gunElevation;
        SmokeController   * m_smoker;
        GunController     * m_airsoftGun;
        TBSMini           * m_soundUnit;
        Timer<>           * m_APTimer;
        bool                m_debugMode;

        void initRadio();
        void initDriveDirection();
        void initTurretRotation();
        void initGunElevation();
        void initSmoker();
        void initAirsoftGun();
        void initSoundUnit();

    public:
        ArduinoPanzer();
        void toggleDebug(bool debugMode);
        void begin(Timer<> * APTimer);
        void readRadio();
        bool foundRadio();
        bool radioButton(uint16_t button);
        int radioTurretRotation();
        int radioGunElevation();
        int radioSteeringThrottle();
        int radioSteeringTurn();
        int radioDriveThrottle();
        int radioDriveReverse();
        int radioDriveLeft();
        int radioDriveRight();
        int maxRadioSpeed();
        DriveDirection    driveDirection();
        DCMotorController turretRotation();
        DCMotorController gunElevation();
        SmokeController * smoker();
        GunController   * airsoftGun();
        TBSMini         * soundUnit();
};

#endif
