#ifndef ARDUINOPANZER_H
#define ARDUINOPANZER_H

#include <Arduino.h>
#include "main.h"
#include <arduino-timer.h>

class ArduinoPanzer {

    private:
        PS2X m_ps2x;
        byte m_ps2xConfigError;
        int m_stickProperties[3] = {MIN_STICK_VALUE, MAX_STICK_VALUE, CENTER_STICK_VALUE};
        byte m_controllerTurretRotation;
        byte m_controllerGunElevation;
        byte m_controllerSteeringThrottle;
        byte m_controllerSteeringTurn;
        byte m_controllerDriveThrottle;
        byte m_controllerDriveReverse;
        byte m_controllerDriveLeft;
        byte m_controllerDriveRight;

        DriveDirection      m_driveDirection;
        DCMotorController   m_turretRotation;
        DCMotorController   m_gunElevation;
        SmokeController   * m_smoker;
        GunController     * m_airsoftGun;
        TBSMini           * m_soundUnit;

        Timer<> * m_APTimer;

        bool m_debugMode;

    public:
        ArduinoPanzer();
        void toggleDebug(bool debugMode);
        void begin(Timer<> * APTimer);
        void initController();
        void readController();
        bool foundController();
        bool controllerButton(uint16_t button);
        byte controllerTurretRotation();
        byte controllerGunElevation();
        byte controllerSteeringThrottle();
        byte controllerSteeringTurn();
        byte controllerDriveThrottle();
        byte controllerDriveReverse();
        byte controllerDriveLeft();
        byte controllerDriveRight();
        byte maxControllerSpeed();
        void initDriveDirection();
        void initTurretRotation();
        void initGunElevation();
        void initSmoker();
        void initAirsoftGun();
        void initSoundUnit();
        DriveDirection    driveDirection();
        DCMotorController turretRotation();
        DCMotorController gunElevation();
        SmokeController * smoker();
        GunController   * airsoftGun();
        TBSMini         * soundUnit();
};

#endif
