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

    Serial.begin(57600);
    Serial.println("Inicializamos Serial");

    driveDirection.begin(false,     // debug pinMode
                          MIN_STICK_VALUE,
                          MAX_STICK_VALUE,
                          true      // Y-axis decreases when I push forward
                          );

    int turretRotationPinout[4] = {TB_TURRETROTATION_PWMA, TB_TURRETROTATION_AIN1, TB_TURRETROTATION_AIN2, TB_TURRETGUN_STBY};
    turretRotation.begin(turretRotationPinout, MIN_STICK_VALUE, MAX_STICK_VALUE, CENTER_STICK_VALUE);
    turretRotation.setMaxVoltagePercent(50);
    turretRotation.enableDebug("Turret rotation");

    int gunElevationPinout[4] = {TB_GUNELEVATION_PWMB, TB_GUNELEVATION_BIN1, TB_GUNELEVATION_BIN2, TB_TURRETGUN_STBY};
    gunElevation.begin(gunElevationPinout, MIN_STICK_VALUE, MAX_STICK_VALUE, CENTER_STICK_VALUE);
    gunElevation.setMaxVoltagePercent(50);
    gunElevation.enableDebug("Gun elevation");

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

    if ((ps2x.Analog(PSS_RX) != CENTER_STICK_VALUE) || (ps2x.Analog(PSS_RY) != CENTER_STICK_VALUE)) {
        turretRotation.move(ps2x.Analog(PSS_RX));
        gunElevation.move(ps2x.Analog(PSS_RY));
    }
    else {
        turretRotation.disableMotor();
        gunElevation.disableMotor();
    }

    if(ps2x.Analog(PSS_LX) != CENTER_STICK_VALUE || ps2x.Analog(PSS_LY) != CENTER_STICK_VALUE) {
        driveDirection.analogMove(ps2x.Analog(PSS_LX), ps2x.Analog(PSS_LY));
    }
    else if (ps2x.Button(PSB_PAD_UP) || ps2x.Button(PSB_PAD_RIGHT) || ps2x.Button(PSB_PAD_LEFT) || ps2x.Button(PSB_PAD_DOWN) ) {
        driveDirection.padMove(ps2x.Analog(PSAB_PAD_UP), ps2x.Analog(PSAB_PAD_DOWN), ps2x.Analog(PSAB_PAD_LEFT), ps2x.Analog(PSAB_PAD_RIGHT));
    }
    else {
        driveDirection.standby(true);
    }

    Serial.println("");
    delay(50);
}
