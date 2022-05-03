#include "ArduinoPanzer.h"
#include "Utilities.h"

/**
 *
 */
ArduinoPanzer::ArduinoPanzer()
{

#ifdef SELECTED_RADIO_TYPE
    m_radio = new PS2Radio();
#else
    m_radio = new nRF24Radio();
#endif
    m_APTimer = new Timer<>();
    m_smoker = new SmokeController();
    m_gunElevation = new GunElevation();
    m_gunRecoil = new GunRecoil();
    m_soundUnit = new TBSMini();
    m_debugMode = false;
}

/**
 *
 */
void ArduinoPanzer::toggleDebug(bool debugMode)
{
    m_debugMode = debugMode;
}

void ArduinoPanzer::begin()
{
    Serial.println("ArduinoPanzer::begin");
    initRadio();
    initTurretRotation();
    initGunElevation();
    initDriveDirection();
    initGunRecoil();
    initSoundUnit();
    initSmoker();
    Serial.println("ArduinoPanzer::begin END");
}

void ArduinoPanzer::update()
{
    m_APTimer->tick();
    m_gunRecoil->update();
    m_gunElevation->update();
    //m_radio->printDebug();
}

void ArduinoPanzer::initRadio()
{
    RadioStickProperties radioStickProperties =
    {
        PS2_MIN_STICK_VALUE,
        PS2_MAX_STICK_VALUE,
        PS2_CENTER_STICK_VALUE,
        true
    };

    m_radio->begin(radioStickProperties);
    m_radio->toggleDebugMode(m_debugMode);
}

void ArduinoPanzer::readRadio()
{
    m_radio->read();

    if (m_debugMode)
    {
        m_radio->printDebug();
    }
}

bool ArduinoPanzer::foundRadio()
{
    return m_radio->isOnline();
}

bool ArduinoPanzer::radioButton(uint16_t button)
{
    return m_radio->button(button);
}

int ArduinoPanzer::radioTurretRotation()   { return m_radio->getTurretRotation(); }
int ArduinoPanzer::radioGunElevation()     { return m_radio->getGunElevation(); }
int ArduinoPanzer::radioSteeringThrottle() { return m_radio->getSteeringThrottle(); }
int ArduinoPanzer::radioSteeringTurn()     { return m_radio->getSteeringTurn(); }
int ArduinoPanzer::radioDriveThrottle()    { return m_radio->getDriveThrottle(); }
int ArduinoPanzer::radioDriveReverse()     { return m_radio->getDriveReverse(); }
int ArduinoPanzer::radioDriveLeft()        { return m_radio->getDriveLeft(); }
int ArduinoPanzer::radioDriveRight()       { return m_radio->getDriveRight(); }

int ArduinoPanzer::maxRadioSpeed()
{
    return m_radio->maxSpeed();
}

void ArduinoPanzer::initDriveDirection()
{
    DCMotorControllerProperties leftMotorConfig =
    {
        TB_DRIVE_APWM_PRESCALER,
        TB_DRIVE_APWM,
        TB_DRIVE_AIN1,
        TB_DRIVE_AIN2,
        TB_DRIVE_STBY
    };

    DCMotorControllerProperties rightMotorConfig =
    {
        TB_DRIVE_BPWM_PRESCALER,
        TB_DRIVE_BPWM,
        TB_DRIVE_BIN1,
        TB_DRIVE_BIN2,
        TB_DRIVE_STBY
    };

    m_driveDirection.begin(leftMotorConfig, rightMotorConfig, m_radio->getStickProperties());
    m_driveDirection.setMaxVoltagePercentAnalogMove(DRIVE_MAX_VOLTAGE_PERCENT_ANALOG);
    m_driveDirection.setMaxVoltagePercentPadMove(DRIVE_MAX_VOLTAGE_PERCENT_PAD);

    if (m_debugMode)
        m_driveDirection.enableDebug();
}

void ArduinoPanzer::initTurretRotation()
{
    DCMotorControllerProperties turretRotationConfig =
    {
        TB_TURRETROTATION_PWM_PRESCALER,
        TB_TURRETROTATION_PWM,
        TB_TURRETROTATION_IN1,
        TB_TURRETROTATION_IN2,
        TB_TURRETROTATION_STBY
    };

    m_turretRotation.begin(turretRotationConfig, m_radio->getStickProperties());
    m_turretRotation.setMaxVoltagePercent(TURRETROTARION_MAX_VOLTAGE_PERCENT);

    if (m_debugMode)
        m_turretRotation.enableDebug("Turret rotation");
}

void ArduinoPanzer::initGunElevation()
{
    GunElevationProperties gunElevationProperties =
    {
        GUNELEVATION_SERVO_PIN,
        GUNELEVATION_DEPRESSION_DEGREES,
        GUNELEVATION_ELEVATION_DEGREES,
        GUNELEVATION_HORIZONTAL_DEGREES,
        GUNELEVATION_DEGREES_INCREMENT,
        GUNELEVATION_MS_INCREMENT
    };

    RadioStickProperties RadioStickProperties =
    {
        NRF24_MIN_STICK_VALUE,
        NRF24_MAX_STICK_VALUE,
        NRF24_CENTER_STICK_VALUE
    };

    m_gunElevation->begin(gunElevationProperties, RadioStickProperties);
}

void ArduinoPanzer::initSmoker()
{
    DCMotorControllerProperties smokerGeneratorConfig =
    {
        TB_SMOKE_GENERATOR_PWM_PRESCALER,
        TB_SMOKE_GENERATOR_PWM,
        TB_SMOKE_GENERATOR_AIN1,
        TB_SMOKE_GENERATOR_AIN2,
        TB_SMOKE_GENERATORFAN_STBY
    };

    DCMotorControllerProperties smokerFanConfig =
    {
        TB_SMOKE_FAN_PWM_PRESCALER,
        TB_SMOKE_FAN_PWM,
        TB_SMOKE_FAN_IN1,
        TB_SMOKE_FAN_IN2,
        TB_SMOKE_GENERATORFAN_STBY
    };

    m_smoker->begin(smokerGeneratorConfig, smokerFanConfig, SMOKE_BEHAVIOUR);
    m_smoker->setGeneratorVoltagesPercent(SMOKE_GENERATOR_MAX_VOLTAGE_PERCENT, SMOKE_GENERATOR_IDLE_VOLTAGE_PERCENT, SMOKE_GENERATOR_MOVING_VOLTAGE_PERCENT);
    m_smoker->setFanVoltagesPercent(SMOKE_FAN_MAX_VOLTAGE_PERCENT, SMOKE_FAN_IDLE_VOLTAGE_PERCENT, SMOKE_FAN_MOVING_VOLTAGE_PERCENT);
    m_smoker->setProportional(NRF24_CENTER_STICK_VALUE, abs(NRF24_MAX_STICK_VALUE));
    m_smoker->idle(); // necesary?

    if (m_debugMode)
        m_smoker->enableDebug();
}

void ArduinoPanzer::initGunRecoil()
{
    GunRecoilProperties recoilProperties =
    {
        GUNRECOIL_SERVO_PIN,
        GUNRECOIL_RECOIL_TIME,
        GUNRECOIL_RETURN_TIME,
        GUNRECOIL_DEGREES_IDLE,
        GUNRECOIL_DEGREES_RECOIL
    };

    m_gunRecoil->begin(m_APTimer, recoilProperties);
}

void ArduinoPanzer::initSoundUnit()
{
    m_soundUnit->begin(m_APTimer, TBSMINI_PROP1, TBSMINI_PROP2, TBSMINI_PROP3, NRF24_CENTER_STICK_VALUE, NRF24_MAX_STICK_VALUE);

    if (m_debugMode)
        m_soundUnit->enableDebug();
}

DriveDirection ArduinoPanzer::driveDirection()
{
    return m_driveDirection;
}

DCMotorController ArduinoPanzer::turretRotation()
{
    return m_turretRotation;
}

GunElevation *ArduinoPanzer::gunElevation()
{
    return m_gunElevation;
}

SmokeController *ArduinoPanzer::smoker()
{
    return m_smoker;
}

GunRecoil *ArduinoPanzer::gunRecoil()
{
    return m_gunRecoil;
}

TBSMini *ArduinoPanzer::soundUnit()
{
    return m_soundUnit;
}
