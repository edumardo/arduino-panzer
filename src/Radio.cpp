#include "Radio.h"

Radio::Radio(){

    m_debugMode = false;
    m_msLastRead = 0;
}

int Radio::maxSpeed(){

    int centerValue = m_stickProperties.centerValue;
    int absX = (m_steeringThrottle < centerValue) ? centerValue - m_steeringThrottle  + centerValue - 1 : m_steeringThrottle;
    int absY = (m_steeringTurn     < centerValue) ? centerValue - m_steeringTurn      + centerValue - 1 : m_steeringTurn;
    return max(absX, absY);
}

void Radio::toggleDebugMode(bool debugMode) {

    m_debugMode = debugMode;
}

RadioStickProperties Radio::getStickProperties() { return m_stickProperties;}
int Radio::getTurretRotation()                   { return m_turretRotation; }
int Radio::getGunElevation()                     { return m_gunElevation; }
int Radio::getSteeringThrottle()                 { return m_steeringThrottle; }
int Radio::getSteeringTurn()                     { return m_steeringTurn; }
int Radio::getDriveThrottle()                    { return m_driveThrottle; }
int Radio::getDriveReverse()                     { return m_driveReverse; }
int Radio::getDriveLeft()                        { return m_driveLeft; }
int Radio::getDriveRight()                       { return m_driveRight; }
