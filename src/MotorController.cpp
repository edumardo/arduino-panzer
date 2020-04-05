#include "MotorController.h"

/**
 * Constructor class.
 */
MotorController::MotorController() {

}

/**
 * Initialize pins and the differential steering object, and turn off motors.
 * Pins are defined in pinout.h
 *
 * @param debug_mode: true to enable serial.print, false otherwise.
 */
void MotorController::begin(bool debug_mode) {

    m_debug_mode = debug_mode;
    m_diffSteer.begin(PIVOT_Y_LIMIT);

    pinMode(TB_PWMA, OUTPUT);
    pinMode(TB_AIN2, OUTPUT);
    pinMode(TB_AIN1, OUTPUT);
    pinMode(TB_BIN1, OUTPUT);
    pinMode(TB_BIN2, OUTPUT);
    pinMode(TB_PWMB, OUTPUT);
    standby(true);
}

/**
 * Use differential steering to move the tank with [x,y] values, i.e., an analog stick.
 *
 * @param x: value of x axis, [0,255].
 * @param y: value of y axis, [0,255].
 */
void MotorController::analogMove(byte x, byte y) {

    byte diffSteerComputeRange = m_diffSteer.getComputeRange();

    // From PS2 range to DifferentialSteering Range.
    m_diffSteer.computeMotors(
        map(x,
            PS2_MIN_VALUE,
            PS2_MAX_VALUE,
            -diffSteerComputeRange,
            diffSteerComputeRange)
        ,
        map(y,
            PS2_MIN_VALUE,
            PS2_MAX_VALUE,
            diffSteerComputeRange,
            -diffSteerComputeRange)
        );

    move(m_diffSteer.computedLeftMotor(), m_diffSteer.computedRightMotor());

    if (m_debug_mode) {
        printXY(x, y);
    }
}

/**
 * Move tank with directional pad values, i.e., a d-pad.
 *
 * @param padUp: up button value, [0, 255].
 * @param padDown: down button value, [0, 255].
 * @param padLeft: left button value, [0, 255].
 * @param padRight: right button value, [0, 255].
 */
void MotorController::padMove(byte padUp, byte padDown, byte padLeft, byte padRight) {

    if (padUp)          move(PAD_FIXED_SPEED, PAD_FIXED_SPEED);
    else if (padDown)   move(-PAD_FIXED_SPEED, -PAD_FIXED_SPEED);
    else if (padLeft)   move(0,  PAD_FIXED_SPEED);
    else if (padRight)  move(PAD_FIXED_SPEED, 0);

    if (m_debug_mode) {
        printXY(padUp, padDown);
        printXY(padLeft, padRight);
    }
}

/**
 * Private method to send PWM values to the motors. Also it controls the direction of both motors.
 *
 * @param leftMotor: left motor value, [-127,127].
 * @param rightMotor: right motor value, [-127,127].
 */
void MotorController::move(int leftMotor, int rightMotor) {

    standby(false);
    digitalWrite(TB_AIN1, (leftMotor  < 0) ? LOW  : HIGH);
    digitalWrite(TB_AIN2, (leftMotor  < 0) ? HIGH : LOW);
    digitalWrite(TB_BIN1, (rightMotor < 0) ? HIGH : LOW);
    digitalWrite(TB_BIN2, (rightMotor < 0) ? LOW  : HIGH);
    int pwml = abs(map(leftMotor,  -127, 127, -255, 255));  // [0,255]
    int pwmr = abs(map(rightMotor, -127, 127, -255, 255));  // [0,255]
    analogWrite(TB_PWMA, pwml);
    analogWrite(TB_PWMB, pwmr);

    if (m_debug_mode) {
        printXY(leftMotor, rightMotor);
        if (leftMotor  < 0) Serial.print("[leftMotor reverse]"); else if (leftMotor == 0) Serial.print("[leftMotor idle]"); else Serial.print("[leftMotor forward]");
        if (rightMotor < 0) Serial.print("[rightMotor reverse]"); else if (rightMotor == 0) Serial.print("[rightMotor idle]"); else Serial.print("[rightMotor forward]");
        printXY(pwml, pwmr);
    }
}

/**
 * Enable or disable motors.
 *
 * @param standby: true to disable motors, false to enable.
 */
void MotorController::standby(bool standby) {

    digitalWrite(TB_STBY, standby ? LOW: HIGH);

    if (m_debug_mode) {
        Serial.print("[MotorController] standbyMotors: "); Serial.println(standby ? "true" : "false");
    }
}

/**
 * Prints [x,y] to serial without new line.
 *
 * @param x: x value to print.
 * @param y: y value to print.
 */
void MotorController::printXY(int x, int y) {
    Serial.print("[");
    Serial.print(x, DEC);
    Serial.print(",");
    Serial.print(y, DEC);
    Serial.print("]");
}

