#include <PS2X_lib.h>
#include <arduino-timer.h>
#include "src/main.h"
//--------------------------------------------------------------------------------------------------------------------------------------------------
void readPS2Controller();
void initDriveDirection();
void initTurretRotation();
void initGunElevation();
void initSmoker();
void initAirsoftGun();
void initSoundUnit();
//--------------------------------------------------------------------------------------------------------------------------------------------------
DriveDirection    driveDirection;
DCMotorController turretRotation;
DCMotorController gunElevation;
SmokeController   smoker;
GunController     airsoftGun;
TBSMiniController soundUnit;

PS2X ps2x;
byte ps2xConfigError = 0;
byte controllerTurretRotation;
byte controllerGunElevation;
byte controllerSteeringThrottle;
byte controllerSteeringTurn;
byte controllerDriveThrottle;
byte controllerDriveReverse;
byte controllerDriveLeft;
byte controllerDriveRight;
int stickProperties[3] = {MIN_STICK_VALUE, MAX_STICK_VALUE, CENTER_STICK_VALUE};

Timer<> APTimer;
byte simuInterrupt = 42;
//--------------------------------------------------------------------------------------------------------------------------------------------------
void setup() {

    Serial.begin(115200);
    Serial.println("Inicializamos Serial");

    initTurretRotation();
    initGunElevation();
    initDriveDirection();
    initAirsoftGun();
    initSoundUnit();
    initSmoker();

    delay(300);  //added delay to give wireless ps2 module some time to startup, before configuring it
    ps2xConfigError = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, PS2_PRESSURES, PS2_RUMBLE);
    if (ps2xConfigError == 0){
        Serial.println("Found Controller, configured successful ");
    } else {
        Serial.println("ps2xConfigError, check wiring");
    }

    uint8_t ps2xType = ps2x.readType();
    Serial.print("PS2X ps2xType: "); 
    Serial.println(ps2xType);
    switch(ps2xType) {
        case 0: Serial.print("Unknown Controller ps2xType found "); break;
        case 1: Serial.print("DualShock Controller found "); break;
        case 2: Serial.print("GuitarHero Controller found "); break;
        case 3: Serial.print("Wireless Sony DualShock Controller found "); break;
    }

    /* Interrupt simulation when airsoft shoot is complete */
    pinMode(simuInterrupt, OUTPUT);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------
void loop() {

    if(ps2xConfigError == 1)
        return;

    readPS2Controller();
    APTimer.tick();

    /* Turret rotation and gun elevation */
    if ((controllerTurretRotation != CENTER_STICK_VALUE) || (controllerGunElevation != CENTER_STICK_VALUE)) {
        turretRotation.move(controllerTurretRotation);
        gunElevation.move(controllerGunElevation);
        soundUnit.playTurretRotation();
    }
    else {
        turretRotation.disableMotor();
        gunElevation.disableMotor();
        soundUnit.stopTurretRotation();
    }

    /* Drive direction */
    if(controllerSteeringThrottle != CENTER_STICK_VALUE || controllerSteeringTurn != CENTER_STICK_VALUE) {
        driveDirection.move(controllerSteeringThrottle, controllerSteeringTurn);
        soundUnit.setEngineSpeed(controllerSteeringThrottle);
        smoker.smoke(controllerSteeringThrottle, controllerSteeringTurn);
    }
    else if (ps2x.Button(CONTROLLER_BUTTON_DRIVE_THROTTLE) || ps2x.Button(CONTROLLER_BUTTON_DRIVE_RIGHT) || ps2x.Button(CONTROLLER_BUTTON_DRIVE_LEFT) || ps2x.Button(CONTROLLER_BUTTON_DRIVE_REVERSE) ) {
        driveDirection.move(controllerDriveThrottle, controllerDriveReverse, controllerDriveLeft, controllerDriveRight);
        soundUnit.setEngineSpeed(90);        // [0, 127]
        smoker.smoke(CONTROLLER_BUTTON_DRIVE_THROTTLE, CONTROLLER_BUTTON_DRIVE_THROTTLE);
    }
    else {
        driveDirection.disableMotors();
        soundUnit.idleEngine();
        smoker.idle();
    }

    if (ps2x.Button(CONTROLLER_FIRE_GUN)) {
        if (airsoftGun.isFired()) {
            airsoftGun.readyToFire();
            soundUnit.playCannon();
        }
        else {
            airsoftGun.fire();
        }
    }

    /* Simulate airsoft ok and throw interrupt */
    if (ps2x.Button(PSB_RED)) {
        if ( digitalRead(simuInterrupt) == LOW) {
            digitalWrite(simuInterrupt, HIGH);
        }
        else {
            digitalWrite(simuInterrupt, LOW);
        }
    }

    if (ps2x.Button(CONTROLLER_STARTSTOP)) {
        if (!soundUnit.isEngingeRunning()) {
            soundUnit.startEngine();
            smoker.start();
        }
    }

    delay(50);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------
/**
 * 
 */
void readPS2Controller() {

    ps2x.read_gamepad();
    controllerTurretRotation   = ps2x.Analog(CONTROLLER_TURRET_ROTATION);
    controllerGunElevation     = ps2x.Analog(CONTROLLER_GUN_ELEVATION);
    controllerSteeringThrottle = ps2x.Analog(CONTROLLER_STEERING_THROTTLE);
    controllerSteeringTurn     = ps2x.Analog(CONTROLLER_STEERING_TURN);
    controllerDriveThrottle    = ps2x.Analog(CONTROLLER_DRIVE_THROTTLE);
    controllerDriveReverse     = ps2x.Analog(CONTROLLER_DRIVE_REVERSE);
    controllerDriveLeft        = ps2x.Analog(CONTROLLER_DRIVE_LEFT);
    controllerDriveRight       = ps2x.Analog(CONTROLLER_DRIVE_RIGHT);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------
/**
 *
 */
void initDriveDirection() {

    DCMotorControllerConfig leftMotorConfig = {TB_DRIVE_APWM_PRESCALER, TB_DRIVE_APWM, TB_DRIVE_AIN1, TB_DRIVE_AIN2, TB_DRIVE_STBY};
    DCMotorControllerConfig rightMotorConfig = {TB_DRIVE_BPWM_PRESCALER, TB_DRIVE_BPWM, TB_DRIVE_BIN1, TB_DRIVE_BIN2, TB_DRIVE_STBY};
    driveDirection.begin(leftMotorConfig, rightMotorConfig, stickProperties);
    driveDirection.invertYStick();
    driveDirection.setMaxVoltagePercentAnalogMove(DRIVE_MAX_VOLTAGE_PERCENT_ANALOG);
    driveDirection.setMaxVoltagePercentPadMove(DRIVE_MAX_VOLTAGE_PERCENT_PAD);
    driveDirection.enableDebug();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------
/**
 *
 */
void initTurretRotation() {

    DCMotorControllerConfig turretRotationConfig {TB_TURRETROTATION_PWM_PRESCALER, TB_TURRETROTATION_PWM, TB_TURRETROTATION_IN1, TB_TURRETROTATION_IN2, TB_TURRETGUN_STBY};
    turretRotation.begin(turretRotationConfig, stickProperties);
    turretRotation.setMaxVoltagePercent(TURRETROTARION_MAX_VOLTAGE_PERCENT);
    turretRotation.enableDebug("Turret rotation");
}

//--------------------------------------------------------------------------------------------------------------------------------------------------
/**
 *
 */
void initGunElevation() {

    DCMotorControllerConfig gunElevationConfig = {TB_GUNELEVATION_PWM_PRESCALER, TB_GUNELEVATION_PWM, TB_GUNELEVATION_IN1, TB_GUNELEVATION_IN2, TB_TURRETGUN_STBY};
    gunElevation.begin(gunElevationConfig, stickProperties);
    gunElevation.setMaxVoltagePercent(GUNELEVATION_MAX_VOLTAGE_PERCENT);
    gunElevation.enableDebug("Gun elevation");
}

//--------------------------------------------------------------------------------------------------------------------------------------------------
/**
 *
 */
void initSmoker() {

    DCMotorControllerConfig smokerGeneratorConfig = {TB_SMOKE_GENERATOR_PWM_PRESCALER, TB_SMOKE_GENERATOR_PWM, TB_SMOKE_GENERATOR_AIN1, TB_SMOKE_GENERATOR_AIN2, TB_SMOKE_GENERATORFAN_STBY};
    DCMotorControllerConfig smokerFanConfig = {TB_SMOKE_FAN_PWM_PRESCALER, TB_SMOKE_FAN_PWM, TB_SMOKE_FAN_IN1, TB_SMOKE_FAN_IN2, TB_SMOKE_GENERATORFAN_STBY};
    smoker.begin(smokerGeneratorConfig, smokerFanConfig, SmokeGeneratorBehaviour::proportional);
    smoker.setGeneratorVoltagesPercent(SMOKE_GENERATOR_MAX_VOLTAGE_PERCENT, SMOKE_GENERATOR_IDLE_VOLTAGE_PERCENT, SMOKE_GENERATOR_MOVING_VOLTAGE_PERCENT);
    smoker.setFanVoltagesPercent(SMOKE_FAN_MAX_VOLTAGE_PERCENT, SMOKE_FAN_IDLE_VOLTAGE_PERCENT, SMOKE_FAN_MOVING_VOLTAGE_PERCENT);
    smoker.setProportional(CENTER_STICK_VALUE, abs(MAX_STICK_VALUE));
    smoker.enableDebug();
    smoker.idle();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------
/**
 *
 */
void initAirsoftGun() {

    airsoftGun.begin(&APTimer, AIRSOFT_FIRED_INTERRUPT_PIN);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------
/**
 *
 */
void initSoundUnit() {

    soundUnit.begin(TBSMINI_PROP1, TBSMINI_PROP2, CENTER_STICK_VALUE, MAX_STICK_VALUE);
}
