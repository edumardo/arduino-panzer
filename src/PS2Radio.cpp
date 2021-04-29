#include "PS2Radio.h"
#include "Utilities.h"

PS2Radio::PS2Radio()
    : Radio() {

    m_msLastRead = PS2_MS_BETWEEN_READS;
}

void PS2Radio::begin(RadioStickProperties radioStickProperties) {

    m_stickProperties = radioStickProperties;

    m_ps2xConfigError = m_ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, PS2_PRESSURES, PS2_RUMBLE);
    if (m_ps2xConfigError == 0) {
        Serial.println("Found Controller, configured successful ");
    } else {
        Serial.println("m_ps2xConfigError, check wiring");
    }

    uint8_t ps2xType = m_ps2x.readType();
    Serial.print("PS2X ps2xType: ");
    Serial.println(ps2xType);
    switch(ps2xType) {
        case 0: Serial.print("Unknown Controller ps2xType found "); break;
        case 1: Serial.print("DualShock Controller found "); break;
        case 2: Serial.print("GuitarHero Controller found "); break;
        case 3: Serial.print("Wireless Sony DualShock Controller found "); break;
    }
}

void PS2Radio::read() {

    
    if (millis() - m_msLastRead > PS2_MS_BETWEEN_READS) {
        m_msLastRead = millis();
        m_ps2x.read_gamepad();
        m_turretRotation   = m_ps2x.Analog(CONTROLLER_TURRET_ROTATION);
        m_gunElevation     = m_ps2x.Analog(CONTROLLER_GUN_ELEVATION);
        m_steeringThrottle = m_ps2x.Analog(CONTROLLER_STEERING_THROTTLE);
        m_steeringTurn     = m_ps2x.Analog(CONTROLLER_STEERING_TURN);
        m_driveThrottle    = m_ps2x.Analog(CONTROLLER_DRIVE_THROTTLE);
        m_driveReverse     = m_ps2x.Analog(CONTROLLER_DRIVE_REVERSE);
        m_driveLeft        = m_ps2x.Analog(CONTROLLER_DRIVE_LEFT);
        m_driveRight       = m_ps2x.Analog(CONTROLLER_DRIVE_RIGHT);
    }
}

bool PS2Radio::button(uint16_t button) {

    return m_ps2x.Button(button);
}

void PS2Radio::printDebug(){

    printAnalogController("LA", m_steeringThrottle, m_steeringTurn);
    printAnalogController("RA", m_turretRotation, m_gunElevation);
    printUDLRController(m_driveThrottle, m_driveReverse, m_driveLeft, m_driveRight);
    Serial.println();
}

bool PS2Radio::isOnline() {

    return (m_ps2xConfigError == 0);
}
