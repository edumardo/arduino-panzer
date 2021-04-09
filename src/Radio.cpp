#include "Radio.h"

Radio::Radio(){

    m_debugMode = false;
}

int Radio::maxSpeed(){

    int stickCenter = m_stickProperties[2];
    int absX = (m_steeringThrottle < stickCenter) ? stickCenter - m_steeringThrottle  + stickCenter - 1 : m_steeringThrottle;
    int absY = (m_steeringTurn     < stickCenter) ? stickCenter - m_steeringTurn      + stickCenter - 1 : m_steeringTurn;
    return max(absX, absY);
}

void Radio::toggleDebugMode(bool debugMode) {

    m_debugMode = debugMode;
}

int * Radio::getStickProperties() { return m_stickProperties;}
int Radio::getTurretRotation()    { return m_turretRotation; }
int Radio::getGunElevation()      { return m_gunElevation; }
int Radio::getSteeringThrottle()  { return m_steeringThrottle; }
int Radio::getSteeringTurn()      { return m_steeringTurn; }
int Radio::getDriveThrottle()     { return m_driveThrottle; }
int Radio::getDriveReverse()      { return m_driveReverse; }
int Radio::getDriveLeft()         { return m_driveLeft; }
int Radio::getDriveRight()        { return m_driveRight; }
