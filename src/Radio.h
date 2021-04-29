#ifndef RADIO_H
#define RADIO_H

#include <Arduino.h>
#include "RadioStickProperties.h"

class Radio {

    protected:
        RadioStickProperties m_stickProperties;
        int m_turretRotation;
        int m_gunElevation;
        int m_steeringThrottle;
        int m_steeringTurn;
        int m_driveThrottle;
        int m_driveReverse;
        int m_driveLeft;
        int m_driveRight;
        bool m_debugMode;
        long m_msLastRead;

    public:
        Radio();
        virtual void begin(RadioStickProperties radioStickProperties) = 0;
        virtual bool isOnline() = 0;
        virtual void read() = 0;
        virtual bool button(uint16_t button) = 0;
        virtual void printDebug() = 0;
        int maxSpeed();
        void toggleDebugMode(bool debugMode);
        RadioStickProperties getStickProperties();
        int getTurretRotation();
        int getGunElevation();
        int getSteeringThrottle();
        int getSteeringTurn();
        int getDriveThrottle();
        int getDriveReverse();
        int getDriveLeft();
        int getDriveRight();
};

#endif
