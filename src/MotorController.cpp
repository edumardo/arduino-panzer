#include "MotorController.h"

/**
 *
 */
MotorController::MotorController()
    : m_diffSteer(PIVOT_Y_LIMIT) {

}

/**
 *
 */
void MotorController::begin(bool debug_mode) {

    m_debug_mode = debug_mode;
    pinMode(TB_PWMA, OUTPUT);
    pinMode(TB_AIN2, OUTPUT);
    pinMode(TB_AIN1, OUTPUT);
    pinMode(TB_BIN1, OUTPUT);
    pinMode(TB_BIN2, OUTPUT);
    pinMode(TB_PWMB, OUTPUT);
    standby(true);
}

/**
 *
 */
void MotorController::analogMove(byte XRaw, byte YRaw) {

    Serial.print("[");Serial.print(XRaw, DEC); Serial.print(","); Serial.print(YRaw, DEC); Serial.print("]");
    int XMap = map(XRaw, 0, 255, -127, 127);
    int YMap = map(YRaw, 0, 255, 127, -127);
    Serial.print("[");Serial.print(XMap, DEC); Serial.print(","); Serial.print(YMap, DEC); Serial.print("]");
    if(XRaw != 128 || YRaw != 128) {
        m_diffSteer.computeMotors(XMap, YMap);
        move(m_diffSteer.computedLeftMotor(), m_diffSteer.computedRightMotor());
    }
    else {
        standby(true);
    }
}

/**
 *
 */
void MotorController::padMove(byte PadUp, byte PadDown, byte PadLeft, byte PadRight) {

    if (PadUp)          move(PAD_FIXED_SPEED, PAD_FIXED_SPEED);
    else if (PadDown)   move(-PAD_FIXED_SPEED, -PAD_FIXED_SPEED);
    else if (PadLeft)   move(0,  PAD_FIXED_SPEED);
    else if (PadRight)  move(PAD_FIXED_SPEED, 0);

    if (m_debug_mode) {
        Serial.print("[");Serial.print(PadUp, DEC); Serial.print(","); Serial.print(PadDown, DEC); Serial.print("]");
        Serial.print("[");Serial.print(PadLeft, DEC); Serial.print(","); Serial.print(PadRight, DEC); Serial.print("]");
    }
}

/**
 *
 */
void MotorController::move(int computedLeft, int computedRight) {

    standby(false);
    digitalWrite(TB_AIN1, (computedLeft  < 0) ? LOW  : HIGH);
    digitalWrite(TB_AIN2, (computedLeft  < 0) ? HIGH : LOW);
    digitalWrite(TB_BIN1, (computedRight < 0) ? HIGH : LOW);
    digitalWrite(TB_BIN2, (computedRight < 0) ? LOW  : HIGH);
    int pwml = abs(map(computedLeft,  -127, 127, -255, 255));
    int pwmr = abs(map(computedRight, -127, 127, -255, 255));
    analogWrite(TB_PWMA, pwml);
    analogWrite(TB_PWMB, pwmr);

    if (m_debug_mode) {
        Serial.print("["); Serial.print(computedLeft, DEC); Serial.print(","); Serial.print(computedRight, DEC); Serial.print("]");
        if (computedLeft  < 0) Serial.print("[leftMotor reverse]"); else if (computedLeft == 0) Serial.print("[leftMotor idle]"); else Serial.print("[leftMotor forward]");
        if (computedRight < 0) Serial.print("[rightMotor reverse]"); else if (computedRight == 0) Serial.print("[rightMotor idle]"); else Serial.print("[rightMotor forward]");
        Serial.print("["); Serial.print(pwml, DEC); Serial.print(","); Serial.print(pwmr, DEC); Serial.print("]");
    }
}

/**
 *
 */
void MotorController::standby(bool standby) {

    digitalWrite(TB_STBY, standby ? LOW: HIGH);

    if (m_debug_mode) {
        Serial.print("[MotorController] standbyMotors: "); Serial.println(standby, BIN);
    }
}

