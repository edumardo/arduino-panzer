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
/*********************************************************************************************************************/
int error = 0;
int type = 0;
int vibrate = 0;

unsigned char XRaw = 0;
unsigned char YRaw = 0;
int XMap = 0;
int YMap = 0;
int computedLeft = 0;
int computedRight = 0;

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

void TB_move(int computedLeft, int computedRight) {

    digitalWrite(TB_AIN1, (computedLeft  < 0) ? LOW  : HIGH);
    digitalWrite(TB_AIN2, (computedLeft  < 0) ? HIGH : LOW);
    digitalWrite(TB_BIN1, (computedRight < 0) ? HIGH : LOW);
    digitalWrite(TB_BIN2, (computedRight < 0) ? LOW  : HIGH);
    int pwml = abs(map(computedLeft,  -127, 127, -255, 255));
    int pwmr = abs(map(computedRight, -127, 127, -255, 255));
    analogWrite(TB_PWMA, pwml);
    analogWrite(TB_PWMB, pwmr);

    Serial.print("["); Serial.print(computedLeft, DEC); Serial.print(","); Serial.print(computedRight, DEC); Serial.print("]");
    if (computedLeft  < 0) Serial.print("[leftMotor reverse]");  else Serial.print("[leftMotor forward]");
    if (computedRight < 0) Serial.print("[rightMotor reverse]"); else Serial.print("[rightMotor forward]");
    Serial.print("["); Serial.print(pwml, DEC); Serial.print(","); Serial.print(pwmr, DEC); Serial.print("]");
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
        case 0:
            Serial.print("Unknown Controller type found "); break;
        case 1:
            Serial.print("DualShock Controller found "); break;
        case 2:
            Serial.print("GuitarHero Controller found "); break;
        case 3:
            Serial.print("Wireless Sony DualShock Controller found "); break;
    }
}
/*********************************************************************************************************************/
void loop() {
    if(error == 1) //skip loop if no controller found
        return;

    ps2x.read_gamepad(); //read controller and set large motor to spin at 'vibrate' speed
    XRaw = ps2x.Analog(PSS_LX);
    YRaw = ps2x.Analog(PSS_LY);
    Serial.print("[");Serial.print(XRaw, DEC); Serial.print(","); Serial.print(YRaw, DEC); Serial.print("]");
    XMap = map(XRaw, 0, 255, -127, 127);
    YMap = map(YRaw, 0, 255, 127, -127);
    Serial.print("[");Serial.print(XMap, DEC); Serial.print(","); Serial.print(YMap, DEC); Serial.print("]");
    if(XRaw != 128 || YRaw != 128) {
        diffSteer.computeMotors(XMap, YMap);
        computedLeft = diffSteer.computedLeftMotor();
        computedRight = diffSteer.computedRightMotor();
        TB_enableMotors();
        TB_move(computedLeft, computedRight);
    }
    else {
        TB_disableMotors();
    }

    Serial.println("");
    delay(50);
}
