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

void ArduinoPanzer::initRadio() {

    m_ps2x.begin();
}

void ArduinoPanzer::readRadio() {

    m_ps2x.read();

    if (m_debugMode) {
        Serial.print(m_ps2x.maxSpeed());
        m_ps2x.printDebug();
    }

}

bool ArduinoPanzer::foundRadio() {

    return m_ps2x.isOnline();
}

bool ArduinoPanzer::radioButton(uint16_t button) {

    return m_ps2x.button(button);
}

int ArduinoPanzer::radioTurretRotation()   { return m_ps2x.getTurretRotation(); }
int ArduinoPanzer::radioGunElevation()     { return m_ps2x.getGunElevation(); }
int ArduinoPanzer::radioSteeringThrottle() { return m_ps2x.getSteeringThrottle(); }
int ArduinoPanzer::radioSteeringTurn()     { return m_ps2x.getSteeringTurn(); }
int ArduinoPanzer::radioDriveThrottle()    { return m_ps2x.getDriveThrottle(); }
int ArduinoPanzer::radioDriveReverse()     { return m_ps2x.getDriveReverse(); }
int ArduinoPanzer::radioDriveLeft()        { return m_ps2x.getDriveLeft(); }
int ArduinoPanzer::radioDriveRight()       { return m_ps2x.getDriveRight(); }

int ArduinoPanzer::maxRadioSpeed(){

    return m_ps2x.maxSpeed();
}

void ArduinoPanzer::initDriveDirection() {

    DCMotorControllerConfig leftMotorConfig = {TB_DRIVE_APWM_PRESCALER, TB_DRIVE_APWM, TB_DRIVE_AIN1, TB_DRIVE_AIN2, TB_DRIVE_STBY};
    DCMotorControllerConfig rightMotorConfig = {TB_DRIVE_BPWM_PRESCALER, TB_DRIVE_BPWM, TB_DRIVE_BIN1, TB_DRIVE_BIN2, TB_DRIVE_STBY};
    m_driveDirection.begin(leftMotorConfig, rightMotorConfig, m_ps2x.getStickProperties());
    m_driveDirection.invertYStick();
    m_driveDirection.setMaxVoltagePercentAnalogMove(DRIVE_MAX_VOLTAGE_PERCENT_ANALOG);
    m_driveDirection.setMaxVoltagePercentPadMove(DRIVE_MAX_VOLTAGE_PERCENT_PAD);

    if (m_debugMode) m_driveDirection.enableDebug();
}

void ArduinoPanzer::initTurretRotation() {

    DCMotorControllerConfig turretRotationConfig {TB_TURRETROTATION_PWM_PRESCALER, TB_TURRETROTATION_PWM, TB_TURRETROTATION_IN1, TB_TURRETROTATION_IN2, TB_TURRETGUN_STBY};
    m_turretRotation.begin(turretRotationConfig, m_ps2x.getStickProperties());
    m_turretRotation.setMaxVoltagePercent(TURRETROTARION_MAX_VOLTAGE_PERCENT);

    if (m_debugMode) m_turretRotation.enableDebug("Turret rotation");
}

void ArduinoPanzer::initGunElevation() {

    DCMotorControllerConfig gunElevationConfig = {TB_GUNELEVATION_PWM_PRESCALER, TB_GUNELEVATION_PWM, TB_GUNELEVATION_IN1, TB_GUNELEVATION_IN2, TB_TURRETGUN_STBY};
    m_gunElevation.begin(gunElevationConfig, m_ps2x.getStickProperties());
    m_gunElevation.setMaxVoltagePercent(GUNELEVATION_MAX_VOLTAGE_PERCENT);

    if (m_debugMode) m_gunElevation.enableDebug("Gun elevation");
}

void ArduinoPanzer::initSmoker() {

    DCMotorControllerConfig smokerGeneratorConfig = {TB_SMOKE_GENERATOR_PWM_PRESCALER, TB_SMOKE_GENERATOR_PWM, TB_SMOKE_GENERATOR_AIN1, TB_SMOKE_GENERATOR_AIN2, TB_SMOKE_GENERATORFAN_STBY};
    DCMotorControllerConfig smokerFanConfig = {TB_SMOKE_FAN_PWM_PRESCALER, TB_SMOKE_FAN_PWM, TB_SMOKE_FAN_IN1, TB_SMOKE_FAN_IN2, TB_SMOKE_GENERATORFAN_STBY};
    m_smoker->begin(smokerGeneratorConfig, smokerFanConfig, SMOKE_BEHAVIOUR);
    m_smoker->setGeneratorVoltagesPercent(SMOKE_GENERATOR_MAX_VOLTAGE_PERCENT, SMOKE_GENERATOR_IDLE_VOLTAGE_PERCENT, SMOKE_GENERATOR_MOVING_VOLTAGE_PERCENT);
    m_smoker->setFanVoltagesPercent(SMOKE_FAN_MAX_VOLTAGE_PERCENT, SMOKE_FAN_IDLE_VOLTAGE_PERCENT, SMOKE_FAN_MOVING_VOLTAGE_PERCENT);
    m_smoker->setProportional(PS2_CENTER_STICK_VALUE, abs(PS2_MAX_STICK_VALUE));
    m_smoker->idle();       // necesary?

    if (m_debugMode) m_smoker->enableDebug();
}

void ArduinoPanzer::initAirsoftGun() {

    m_airsoftGun->begin(m_APTimer, AIRSOFT_FIRED_INTERRUPT_PIN);
}

void ArduinoPanzer::initSoundUnit() {

    m_soundUnit->begin(m_APTimer, TBSMINI_PROP1, TBSMINI_PROP2, TBSMINI_PROP3, PS2_CENTER_STICK_VALUE, PS2_MAX_STICK_VALUE);

    if (m_debugMode)
        m_soundUnit->enableDebug();
}

DriveDirection ArduinoPanzer::driveDirection()    { return m_driveDirection; }
DCMotorController ArduinoPanzer::turretRotation() { return m_turretRotation; }
DCMotorController ArduinoPanzer::gunElevation()   { return m_gunElevation; }
SmokeController * ArduinoPanzer::smoker()         { return m_smoker; }
GunController * ArduinoPanzer::airsoftGun()         { return m_airsoftGun; }
TBSMini * ArduinoPanzer::soundUnit()              { return m_soundUnit; }
