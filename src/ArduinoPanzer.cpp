#include "ArduinoPanzer.h"
#include "Utilities.h"

/**
 *
 */
ArduinoPanzer::ArduinoPanzer() {

    m_smoker = new SmokeController();
    m_airsoftGun = new GunController();
    m_soundUnit = new TBSMini();
    m_debugMode = false;
}

/**
 *
 */
void ArduinoPanzer::toggleDebug(bool debugMode) {

    m_debugMode = debugMode;
}

void ArduinoPanzer::begin(Timer<> * APTimer) {

    m_APTimer = APTimer;
}

void ArduinoPanzer::initController() {

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

void ArduinoPanzer::readController() {

    m_ps2x.read_gamepad();
    m_controllerTurretRotation   = m_ps2x.Analog(CONTROLLER_TURRET_ROTATION);
    m_controllerGunElevation     = m_ps2x.Analog(CONTROLLER_GUN_ELEVATION);
    m_controllerSteeringThrottle = m_ps2x.Analog(CONTROLLER_STEERING_THROTTLE);
    m_controllerSteeringTurn     = m_ps2x.Analog(CONTROLLER_STEERING_TURN);
    m_controllerDriveThrottle    = m_ps2x.Analog(CONTROLLER_DRIVE_THROTTLE);
    m_controllerDriveReverse     = m_ps2x.Analog(CONTROLLER_DRIVE_REVERSE);
    m_controllerDriveLeft        = m_ps2x.Analog(CONTROLLER_DRIVE_LEFT);
    m_controllerDriveRight       = m_ps2x.Analog(CONTROLLER_DRIVE_RIGHT);

    //printAnalogController("LA", m_controllerSteeringThrottle, m_controllerSteeringTurn);
    //Serial.println(maxControllerSpeed());

    if (m_debugMode) {
        printAnalogController("LA", m_controllerSteeringThrottle, m_controllerSteeringTurn);
        printAnalogController("RA", m_controllerTurretRotation, m_controllerGunElevation);
        printUDLRController(m_controllerDriveThrottle, m_controllerDriveReverse, m_controllerDriveLeft, m_controllerDriveRight);
        Serial.println();
    }
}

bool ArduinoPanzer::foundController() {

    return (m_ps2xConfigError == 0);
}

bool ArduinoPanzer::controllerButton(uint16_t button) {

    return m_ps2x.Button(button);
}

byte ArduinoPanzer::controllerTurretRotation()   { return m_controllerTurretRotation; }
byte ArduinoPanzer::controllerGunElevation()     { return m_controllerGunElevation; }
byte ArduinoPanzer::controllerSteeringThrottle() { return m_controllerSteeringThrottle; }
byte ArduinoPanzer::controllerSteeringTurn()     { return m_controllerSteeringTurn; }
byte ArduinoPanzer::controllerDriveThrottle()    { return m_controllerDriveThrottle; }
byte ArduinoPanzer::controllerDriveReverse()     { return m_controllerDriveReverse; }
byte ArduinoPanzer::controllerDriveLeft()        { return m_controllerDriveLeft; }
byte ArduinoPanzer::controllerDriveRight()       { return m_controllerDriveRight; }

byte ArduinoPanzer::maxControllerSpeed(){ 

    byte stickCenter = m_stickProperties[2];
    byte absX = (m_controllerSteeringThrottle < stickCenter) ? stickCenter - m_controllerSteeringThrottle  + stickCenter - 1 : m_controllerSteeringThrottle;
    byte absY = (m_controllerSteeringTurn     < stickCenter) ? stickCenter - m_controllerSteeringTurn      + stickCenter - 1 : m_controllerSteeringTurn;
    return max(absX, absY);
}

void ArduinoPanzer::initDriveDirection() {

    DCMotorControllerConfig leftMotorConfig = {TB_DRIVE_APWM_PRESCALER, TB_DRIVE_APWM, TB_DRIVE_AIN1, TB_DRIVE_AIN2, TB_DRIVE_STBY};
    DCMotorControllerConfig rightMotorConfig = {TB_DRIVE_BPWM_PRESCALER, TB_DRIVE_BPWM, TB_DRIVE_BIN1, TB_DRIVE_BIN2, TB_DRIVE_STBY};
    m_driveDirection.begin(leftMotorConfig, rightMotorConfig, m_stickProperties);
    m_driveDirection.invertYStick();
    m_driveDirection.setMaxVoltagePercentAnalogMove(DRIVE_MAX_VOLTAGE_PERCENT_ANALOG);
    m_driveDirection.setMaxVoltagePercentPadMove(DRIVE_MAX_VOLTAGE_PERCENT_PAD);
    
    if (m_debugMode) m_driveDirection.enableDebug();
}

void ArduinoPanzer::initTurretRotation() {

    DCMotorControllerConfig turretRotationConfig {TB_TURRETROTATION_PWM_PRESCALER, TB_TURRETROTATION_PWM, TB_TURRETROTATION_IN1, TB_TURRETROTATION_IN2, TB_TURRETGUN_STBY};
    m_turretRotation.begin(turretRotationConfig, m_stickProperties);
    m_turretRotation.setMaxVoltagePercent(TURRETROTARION_MAX_VOLTAGE_PERCENT);

    if (m_debugMode) m_turretRotation.enableDebug("Turret rotation");
}

void ArduinoPanzer::initGunElevation() {

    DCMotorControllerConfig gunElevationConfig = {TB_GUNELEVATION_PWM_PRESCALER, TB_GUNELEVATION_PWM, TB_GUNELEVATION_IN1, TB_GUNELEVATION_IN2, TB_TURRETGUN_STBY};
    m_gunElevation.begin(gunElevationConfig, m_stickProperties);
    m_gunElevation.setMaxVoltagePercent(GUNELEVATION_MAX_VOLTAGE_PERCENT);

    if (m_debugMode) m_gunElevation.enableDebug("Gun elevation");
}

void ArduinoPanzer::initSmoker() {

    DCMotorControllerConfig smokerGeneratorConfig = {TB_SMOKE_GENERATOR_PWM_PRESCALER, TB_SMOKE_GENERATOR_PWM, TB_SMOKE_GENERATOR_AIN1, TB_SMOKE_GENERATOR_AIN2, TB_SMOKE_GENERATORFAN_STBY};
    DCMotorControllerConfig smokerFanConfig = {TB_SMOKE_FAN_PWM_PRESCALER, TB_SMOKE_FAN_PWM, TB_SMOKE_FAN_IN1, TB_SMOKE_FAN_IN2, TB_SMOKE_GENERATORFAN_STBY};
    m_smoker->begin(smokerGeneratorConfig, smokerFanConfig, SMOKE_BEHAVIOUR);
    m_smoker->setGeneratorVoltagesPercent(SMOKE_GENERATOR_MAX_VOLTAGE_PERCENT, SMOKE_GENERATOR_IDLE_VOLTAGE_PERCENT, SMOKE_GENERATOR_MOVING_VOLTAGE_PERCENT);
    m_smoker->setFanVoltagesPercent(SMOKE_FAN_MAX_VOLTAGE_PERCENT, SMOKE_FAN_IDLE_VOLTAGE_PERCENT, SMOKE_FAN_MOVING_VOLTAGE_PERCENT);
    m_smoker->setProportional(CENTER_STICK_VALUE, abs(MAX_STICK_VALUE));
    m_smoker->idle();       // necesary?
    
    if (m_debugMode) m_smoker->enableDebug();
}

void ArduinoPanzer::initAirsoftGun() {

    m_airsoftGun->begin(m_APTimer, AIRSOFT_FIRED_INTERRUPT_PIN);
}

void ArduinoPanzer::initSoundUnit() {

    m_soundUnit->begin(m_APTimer, TBSMINI_PROP1, TBSMINI_PROP2, TBSMINI_PROP3, CENTER_STICK_VALUE, MAX_STICK_VALUE);
    
    if (m_debugMode) m_soundUnit->enableDebug();
}

DriveDirection ArduinoPanzer::driveDirection()    { return m_driveDirection; }
DCMotorController ArduinoPanzer::turretRotation() { return m_turretRotation; }
DCMotorController ArduinoPanzer::gunElevation()   { return m_gunElevation; }
SmokeController * ArduinoPanzer::smoker()         { return m_smoker; }
GunController * ArduinoPanzer::airsoftGun()         { return m_airsoftGun; }
TBSMini * ArduinoPanzer::soundUnit()              { return m_soundUnit; }
