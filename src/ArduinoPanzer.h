#ifndef ARDUINOPANZER_H
#define ARDUINOPANZER_H

#include <Arduino.h>
#include "config.h"
#include <arduino-timer.h>
#include "PS2Radio.h"
#include "DriveDirection.h"
#include "DCMotorController.h"
#include "GunElevation.h"
#include "SmokeController.h"
#include "GunRecoil.h"
#include <TBSMini.h>
#include "RadioStickProperties.h"
#include "GunElevationProperties.h"
#include "DCMotorControllerProperties.h"
#include "GunRecoilProperties.h"

class ArduinoPanzer {

    private:
        PS2Radio            m_ps2x;
        DriveDirection      m_driveDirection;
        DCMotorController   m_turretRotation;
        GunElevation      * m_gunElevation;
        SmokeController   * m_smoker;
        GunRecoil         * m_gunRecoil;
        TBSMini           * m_soundUnit;
        Timer<>           * m_APTimer;
        bool                m_debugMode;

        void initRadio();
        void initDriveDirection();
        void initTurretRotation();
        void initGunElevation();
        void initSmoker();
        void initGunRecoil();
        void initSoundUnit();

    public:
        ArduinoPanzer();
        void toggleDebug(bool debugMode);
        void begin();
        void update();
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
        GunElevation    * gunElevation();
        SmokeController * smoker();
        GunRecoil       * gunRecoil();
        TBSMini         * soundUnit();
};

#endif
