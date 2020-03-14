#include <PS2X_lib.h>
#include "DifferentialSteering.h"
/*********************************************************************************************************************/
/* PS2 pins */
#define PS2_PIN_DAT     12
#define PS2_PIN_CMD     11
#define PS2_PIN_SEL     10
#define PS2_PIN_CLK     13
#define PS2_PRESSURES   true
#define PS2_RUMBLE      true

/* TB6612FNG pins */
#define TB_PWMA         3
#define TB_AIN2         2
#define TB_AIN1         4
#define TB_STBY         5
#define TB_BIN1         6
#define TB_BIN2         7
#define TB_PWMB         9

/* Others */
#define BAUD_RATE       57600
#define JOYSTICK_MIDDLE 127
#define RANGE_255       255
/*********************************************************************************************************************/
int error = 0;
int type = 0;
int vibrate = 0;

int fPivYLimit = 32;
DifferentialSteering diffSteer(fPivYLimit);
PS2X ps2x;
/*********************************************************************************************************************/
void TB_begin() {

    Serial.println("Inicializamos pins TB");
    pinMode(TB_PWMA, OUTPUT);
    pinMode(TB_AIN2, OUTPUT);
    pinMode(TB_AIN1, OUTPUT);
    pinMode(TB_BIN1, OUTPUT);
    pinMode(TB_BIN2, OUTPUT);
    pinMode(TB_PWMB, OUTPUT);
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
    if (PadUp)          TBMove( 95,  95);   // 95 is 75% of 127
    else if (PadDown)   TBMove(-95, -95);
    else if (PadLeft)   TBMove(  0,  95);
    else if (PadRight)  TBMove( 95,   0);
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
