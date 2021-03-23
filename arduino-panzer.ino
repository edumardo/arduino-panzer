#include <PS2X_lib.h>
#include <arduino-timer.h>
#include "src/main.h"
/*********************************************************************************************************************/
int ps2xConfigError = 0;
int ps2xType = 0;

DriveDirection    driveDirection;
DCMotorController turretRotation;
DCMotorController gunElevation;
SmokeController   smoker;
GunController     airsoftGun;
TBSMiniController soundUnit;
PS2X ps2x;
Timer<> APTimer;
byte simuInterrupt = 42;
/*********************************************************************************************************************/
void setup() {

    Serial.begin(115200);
    Serial.println("Inicializamos Serial");

    /* Stick controller properties */
    int stickProperties[3] = {MIN_STICK_VALUE, MAX_STICK_VALUE, CENTER_STICK_VALUE};

    /* Turret rotation */
    DCMotorControllerConfig turretRotationConfig {TB_TURRETROTATION_PWM_PRESCALER, TB_TURRETROTATION_PWM, TB_TURRETROTATION_IN1, TB_TURRETROTATION_IN2, TB_TURRETGUN_STBY};
    turretRotation.begin(turretRotationConfig, stickProperties);
    turretRotation.setMaxVoltagePercent(100);
    turretRotation.enableDebug("Turret rotation");

    /* Gun elevation */
    DCMotorControllerConfig gunElevationConfig = {TB_GUNELEVATION_PWM_PRESCALER, TB_GUNELEVATION_PWM, TB_GUNELEVATION_IN1, TB_GUNELEVATION_IN2, TB_TURRETGUN_STBY};
    gunElevation.begin(gunElevationConfig, stickProperties);
    gunElevation.setMaxVoltagePercent(100);
    gunElevation.enableDebug("Gun elevation");

    /* Drive direction */
    DCMotorControllerConfig leftMotorConfig = {TB_DRIVE_APWM_PRESCALER, TB_DRIVE_APWM, TB_DRIVE_AIN1, TB_DRIVE_AIN2, TB_DRIVE_STBY};
    DCMotorControllerConfig rightMotorConfig = {TB_DRIVE_BPWM_PRESCALER, TB_DRIVE_BPWM, TB_DRIVE_BIN1, TB_DRIVE_BIN2, TB_DRIVE_STBY};
    driveDirection.begin(leftMotorConfig, rightMotorConfig, stickProperties);
    driveDirection.invertYStick();
    driveDirection.setMaxVoltagePercentAnalogMove(100);
    driveDirection.setMaxVoltagePercentPadMove(80);
    driveDirection.enableDebug();

    /* Airsoft gun */
    airsoftGun.begin(&APTimer, AIRSOFT_FIRED_INTERRUPT_PIN);

    /* Sound handler */
    soundUnit.begin(TBSMINI_PROP1, TBSMINI_PROP2, CENTER_STICK_VALUE, MAX_STICK_VALUE);

    /* Smoke generator and fan */
    DCMotorControllerConfig smokerGeneratorConfig = {TB_SMOKE_GENERATOR_PWM_PRESCALER, TB_SMOKE_GENERATOR_PWM, TB_SMOKE_GENERATOR_AIN1, TB_SMOKE_GENERATOR_AIN2, TB_SMOKE_GENERATORFAN_STBY};
    DCMotorControllerConfig smokerFanConfig = {TB_SMOKE_FAN_PWM_PRESCALER, TB_SMOKE_FAN_PWM, TB_SMOKE_FAN_IN1, TB_SMOKE_FAN_IN2, TB_SMOKE_GENERATORFAN_STBY};
    smoker.begin(smokerGeneratorConfig, smokerFanConfig, SmokeGeneratorBehaviour::proportional);
    smoker.setGeneratorVoltagesPercent(100, 50, 100);
    smoker.setFanVoltagesPercent(100, 50, 50);
    smoker.setProportional(CENTER_STICK_VALUE, abs(MAX_STICK_VALUE));
    smoker.enableDebug();
    smoker.idle();


    delay(300);  //added delay to give wireless ps2 module some time to startup, before configuring it
    ps2xConfigError = ps2x.config_gamepad(PS2_PIN_CLK, PS2_PIN_CMD, PS2_PIN_SEL, PS2_PIN_DAT, PS2_PRESSURES, PS2_RUMBLE);
    if (ps2xConfigError == 0){
        Serial.println("Found Controller, configured successful ");
    } else {
        Serial.println("ps2xConfigError, check wiring");
    }

    ps2xType = ps2x.readType();
    Serial.print("PS2X ps2xType: "); Serial.println(ps2xType);
    switch(ps2xType) {
        case 0: Serial.print("Unknown Controller ps2xType found "); break;
        case 1: Serial.print("DualShock Controller found "); break;
        case 2: Serial.print("GuitarHero Controller found "); break;
        case 3: Serial.print("Wireless Sony DualShock Controller found "); break;
    }

    /* Interrupt simulation when airsoft shoot is complete */
    pinMode(simuInterrupt, OUTPUT);
}

void loop() {

    if(ps2xConfigError == 1)
        return;

    APTimer.tick();

    ps2x.read_gamepad();
    byte pss_rx         = ps2x.Analog(PSS_RX);
    byte pss_ry         = ps2x.Analog(PSS_RY);
    byte pss_lx         = ps2x.Analog(PSS_LX);
    byte pss_ly         = ps2x.Analog(PSS_LY);
    byte psab_pad_up    = ps2x.Analog(PSAB_PAD_UP);
    byte psab_pad_down  = ps2x.Analog(PSAB_PAD_DOWN);
    byte psab_pad_left  = ps2x.Analog(PSAB_PAD_LEFT);
    byte psab_pad_right = ps2x.Analog(PSAB_PAD_RIGHT);

    /* Turret rotation and gun elevation */
    if ((pss_rx != CENTER_STICK_VALUE) || (pss_ry != CENTER_STICK_VALUE)) {
        turretRotation.move(pss_rx);
        gunElevation.move(pss_ry);
        soundUnit.playTurretRotation();
    }
    else {
        turretRotation.disableMotor();
        gunElevation.disableMotor();
        soundUnit.stopTurretRotation();
    }

    /* Drive direction */
    if(pss_lx != CENTER_STICK_VALUE || pss_ly != CENTER_STICK_VALUE) {
        driveDirection.move(pss_lx, pss_ly);
        soundUnit.setEngineSpeed(pss_lx);
        smoker.smoke(pss_lx, pss_ly);
    }
    else if (ps2x.Button(PSB_PAD_UP) || ps2x.Button(PSB_PAD_RIGHT) || ps2x.Button(PSB_PAD_LEFT) || ps2x.Button(PSB_PAD_DOWN) ) {
        driveDirection.move(psab_pad_up, psab_pad_down, psab_pad_left, psab_pad_right);
        soundUnit.setEngineSpeed(90);        // [0, 127]
        smoker.smoke(PSB_PAD_UP, PSB_PAD_UP);
    }
    else {
        driveDirection.disableMotors();
        soundUnit.idleEngine();
        smoker.idle();
    }

    /* Shoot button */
    if (ps2x.Button(PSB_PINK)) {
            if (airsoftGun.isFired()) {
                airsoftGun.readyToFire();
                soundUnit.playCannon();
            }
            else {
                airsoftGun.fire();
            }
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

    /* Start engine */
    if (ps2x.Button(PSB_GREEN)) {}
        if (!soundUnit.isEngingeRunning()) {
            soundUnit.startEngine();
            smoker.start();
        }
    }

    delay(50);
}
