#include <PS2X_lib.h>
#include "DifferentialSteering.h"
#include "src/main.h"
/*********************************************************************************************************************/
int ps2xConfigError = 0;
int ps2xType = 0;

DriveDirection driveDirection;
DCMotorController turretRotation;
DCMotorController gunElevation;
PS2X ps2x;
/*********************************************************************************************************************/
void setup() {

    Serial.begin(115200);
    Serial.println("Inicializamos Serial");

    int stickProperties[3] = {MIN_STICK_VALUE, MAX_STICK_VALUE, CENTER_STICK_VALUE};

    int turretRotationPinout[4] = {TB_TURRETROTATION_PWMA, TB_TURRETROTATION_AIN1, TB_TURRETROTATION_AIN2, TB_TURRETGUN_STBY};
    turretRotation.begin(turretRotationPinout, stickProperties);
    turretRotation.setMaxVoltagePercent(50);
    turretRotation.enableDebug("Turret rotation");

    int gunElevationPinout[4] = {TB_GUNELEVATION_PWMB, TB_GUNELEVATION_BIN1, TB_GUNELEVATION_BIN2, TB_TURRETGUN_STBY};
    gunElevation.begin(gunElevationPinout, stickProperties);
    gunElevation.setMaxVoltagePercent(50);
    gunElevation.enableDebug("Gun elevation");

    int leftMotorPinout[4] = {TB_DRIVE_PWMA, TB_DRIVE_AIN1, TB_DRIVE_AIN2, TB_DRIVE_STBY};
    int RightMotorPinout[4] = {TB_DRIVE_PWMB, TB_DRIVE_BIN1, TB_DRIVE_BIN2, TB_DRIVE_STBY};
    driveDirection.begin(leftMotorPinout, RightMotorPinout, stickProperties);
    driveDirection.invertYStick();
    driveDirection.setMaxVoltagePercentAnalogMove(50);
    driveDirection.setMaxVoltagePercentPadMove(40);
    driveDirection.enableDebug();

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
}
/*********************************************************************************************************************/
void loop() {

    if(ps2xConfigError == 1) //skip loop if no controller found
        return;

    ps2x.read_gamepad();

    byte pss_rx         = ps2x.Analog(PSS_RX);
    byte pss_ry         = ps2x.Analog(PSS_RY);
    byte pss_lx         = ps2x.Analog(PSS_LX);
    byte pss_ly         = ps2x.Analog(PSS_LY);
    byte psab_pad_up    = ps2x.Analog(PSAB_PAD_UP);
    byte psab_pad_down  = ps2x.Analog(PSAB_PAD_DOWN);
    byte psab_pad_left  = ps2x.Analog(PSAB_PAD_LEFT);
    byte psab_pad_right = ps2x.Analog(PSAB_PAD_RIGHT);

    if ((pss_rx != CENTER_STICK_VALUE) || (pss_ry != CENTER_STICK_VALUE)) {
        turretRotation.move(pss_rx);
        gunElevation.move(pss_ry);
    }
    else {
        turretRotation.disableMotor();
        gunElevation.disableMotor();
    }


    if(pss_lx != CENTER_STICK_VALUE || pss_ly != CENTER_STICK_VALUE) {
        driveDirection.move(pss_lx, pss_ly);
    }
    else if (ps2x.Button(PSB_PAD_UP) || ps2x.Button(PSB_PAD_RIGHT) || ps2x.Button(PSB_PAD_LEFT) || ps2x.Button(PSB_PAD_DOWN) ) {
        driveDirection.move(psab_pad_up, psab_pad_down, psab_pad_left, psab_pad_right);
    }
    else {
        driveDirection.disableMotors();
    }

    delay(50);
}
