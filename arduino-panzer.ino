#include <PS2X_lib.h>
#include "DifferentialSteering.h"
#include "src/main.h"
/*********************************************************************************************************************/
/* Others */
#define JOYSTICK_MIDDLE 127
#define PAD_FIXED_SPEED 90          // 95 is 75% of 127
/*********************************************************************************************************************/
int error = 0;
int type = 0;
int vibrate = 0;

int fPivYLimit = 32;
DifferentialSteering diffSteer(fPivYLimit);
PS2X ps2x;

byte ledHullMGState = LOW;
unsigned long ledHullMGpreviousMillis = 0;

/*********************************************************************************************************************/
void TB_begin() {

    Serial.println("Inicializamos pins TB");
    pinMode(TB_PWMA, OUTPUT);
    pinMode(TB_AIN2, OUTPUT);
    pinMode(TB_AIN1, OUTPUT);
    pinMode(TB_BIN1, OUTPUT);
    pinMode(TB_BIN2, OUTPUT);
    pinMode(TB_PWMB, OUTPUT);
    pinMode(LED_HULLMG_PIN, OUTPUT);
}

void TB_enableMotors() {

    //Serial.print("TB_enableMotors");
    digitalWrite(TB_STBY, HIGH);
}

void TB_disableMotors() {

    //Serial.print("TB_disableMotors");
    digitalWrite(TB_STBY, LOW);
}

void TBMove (int computedLeft, int computedRight) {

    TB_enableMotors();
    digitalWrite(TB_AIN1, (computedLeft  < 0) ? LOW  : HIGH);
    digitalWrite(TB_AIN2, (computedLeft  < 0) ? HIGH : LOW);
    digitalWrite(TB_BIN1, (computedRight < 0) ? HIGH : LOW);
    digitalWrite(TB_BIN2, (computedRight < 0) ? LOW  : HIGH);
    int pwml = abs(map(computedLeft,  -127, 127, -255, 255));
    int pwmr = abs(map(computedRight, -127, 127, -255, 255));
    analogWrite(TB_PWMA, pwml);
    analogWrite(TB_PWMB, pwmr);

    Serial.print("["); Serial.print(computedLeft, DEC); Serial.print(","); Serial.print(computedRight, DEC); Serial.print("]");
    if (computedLeft  < 0) Serial.print("[leftMotor reverse]"); else if (computedLeft == 0) Serial.print("[leftMotor idle]"); else Serial.print("[leftMotor forward]");
    if (computedRight < 0) Serial.print("[rightMotor reverse]"); else if (computedRight == 0) Serial.print("[rightMotor idle]"); else Serial.print("[rightMotor forward]");
    Serial.print("["); Serial.print(pwml, DEC); Serial.print(","); Serial.print(pwmr, DEC); Serial.print("]");
}

void TBAnalogMove(byte XRaw, byte YRaw) {

    Serial.print("[");Serial.print(XRaw, DEC); Serial.print(","); Serial.print(YRaw, DEC); Serial.print("]");
    int XMap = map(XRaw, 0, 255, -127, 127);
    int YMap = map(YRaw, 0, 255, 127, -127);
    Serial.print("[");Serial.print(XMap, DEC); Serial.print(","); Serial.print(YMap, DEC); Serial.print("]");
    if(XRaw != 128 || YRaw != 128) {
        diffSteer.computeMotors(XMap, YMap);
        TBMove(diffSteer.computedLeftMotor(), diffSteer.computedRightMotor());
    }
    else {
        TB_disableMotors();
    }
}

void TBPadMove(byte PadUp, byte PadDown, byte PadLeft, byte PadRight) {

    if (PadUp)          TBMove(PAD_FIXED_SPEED, PAD_FIXED_SPEED);
    else if (PadDown)   TBMove(-PAD_FIXED_SPEED, -PAD_FIXED_SPEED);
    else if (PadLeft)   TBMove(0,  PAD_FIXED_SPEED);
    else if (PadRight)  TBMove(PAD_FIXED_SPEED, 0);
}
/*********************************************************************************************************************/
void setup() {

    Serial.begin(BAUD_RATE);
    Serial.println("Inicializamos Serial");

    TB_begin();

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

    if(ps2x.Button(PSB_L1)) {
        TBAnalogMove(ps2x.Analog(PSS_LX), ps2x.Analog(PSS_LY));
    }
    else if (ps2x.Button(PSB_PAD_UP) || ps2x.Button(PSB_PAD_RIGHT) || ps2x.Button(PSB_PAD_LEFT) || ps2x.Button(PSB_PAD_DOWN) ) {
        TBPadMove(ps2x.Analog(PSAB_PAD_UP), ps2x.Analog(PSAB_PAD_DOWN), ps2x.Analog(PSAB_PAD_LEFT), ps2x.Analog(PSAB_PAD_RIGHT));
    }
    else {
        TB_disableMotors();
    }

    Serial.println("");
    delay(50);
}
