#include <PS2X_lib.h>
#include "DifferentialSteering.h"
#include "src/main.h"
/*********************************************************************************************************************/
/* Debug mode */
#define MOTORCONTROLLER_DEBUG true
/*********************************************************************************************************************/
int error = 0;
int type = 0;
int vibrate = 0;

MotorController motorController;
PS2X ps2x;

byte ledHullMGState = LOW;
unsigned long ledHullMGpreviousMillis = 0;
/*********************************************************************************************************************/
void setup() {

    Serial.begin(BAUD_RATE);
    Serial.println("Inicializamos Serial");

    motorController.begin(MOTORCONTROLLER_DEBUG);

    pinMode(LED_HULLMG_PIN, OUTPUT);

    delay(300);  //added delay to give wireless ps2 module some time to startup, before configuring it
    error = ps2x.config_gamepad(PS2_PIN_CLK, PS2_PIN_CMD, PS2_PIN_SEL, PS2_PIN_DAT, PS2_PRESSURES, PS2_RUMBLE);
    if (error == 0){
        Serial.println("Found Controller, configured successful ");
    } else {
        Serial.println("Error, check wiring");
    }

    type = ps2x.readType();
    Serial.print("PS2X type: "); Serial.println(type);
    switch(type) {
        case 0: Serial.print("Unknown Controller type found "); break;
        case 1: Serial.print("DualShock Controller found "); break;
        case 2: Serial.print("GuitarHero Controller found "); break;
        case 3: Serial.print("Wireless Sony DualShock Controller found "); break;
    }
}
/*********************************************************************************************************************/
void loop() {
    if(error == 1) //skip loop if no controller found
        return;

    ps2x.read_gamepad(); //read controller and set large motor to spin at 'vibrate' speed

    unsigned long currentMillis = millis();
    if (ps2x.Button(PSB_R2)) {
        unsigned long currentMillis = millis();
        if (currentMillis - ledHullMGpreviousMillis >= LED_HULLMG_INTERVAL) {
            ledHullMGpreviousMillis = currentMillis;

            // if the LED is off turn it on and vice-versa:
            if (ledHullMGState == LOW) {
                ledHullMGState = HIGH;
            } else {
                ledHullMGState = LOW;
            }

            digitalWrite(LED_HULLMG_PIN, ledHullMGState);
        }
    }
    if (!ps2x.Button(PSB_R2)) {
        digitalWrite(LED_HULLMG_PIN, LOW);
    }

    if(ps2x.Analog(PSS_LX) != PS2_MIDDLE_VALUE || ps2x.Analog(PSS_LY) != PS2_MIDDLE_VALUE) {
        motorController.analogMove(ps2x.Analog(PSS_LX), ps2x.Analog(PSS_LY));
    }
    else if (ps2x.Button(PSB_PAD_UP) || ps2x.Button(PSB_PAD_RIGHT) || ps2x.Button(PSB_PAD_LEFT) || ps2x.Button(PSB_PAD_DOWN) ) {
        motorController.padMove(ps2x.Analog(PSAB_PAD_UP), ps2x.Analog(PSAB_PAD_DOWN), ps2x.Analog(PSAB_PAD_LEFT), ps2x.Analog(PSAB_PAD_RIGHT));
    }
    else {
        motorController.standby(true);
    }

    Serial.println("");
    delay(50);
}
