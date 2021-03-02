#include <PS2X_lib.h>
#include "DifferentialSteering.h"
#include "src/main.h"
/*********************************************************************************************************************/
int ps2xConfigError = 0;
int ps2xType = 0;

DriveDirection driveDirection;
PS2X ps2x;
/*********************************************************************************************************************/
void setup() {

    Serial.begin(57600);
    Serial.println("Inicializamos Serial");

    driveDirection.begin(false,     // debug pinMode
                          0,        // lowest value of the transmitter
                          255,      // highest value of the transmitter
                          true      // Y-axis decreases when I push forward
                          );

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

    if(ps2x.Analog(PSS_LX) != PS2_MIDDLE_VALUE || ps2x.Analog(PSS_LY) != PS2_MIDDLE_VALUE) {
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
