#include "MotorController.h"

/**
 * Constructor class.
 */
MotorController::MotorController() {

}

/**
 * Initialize pins, interval values of the controller, and the direcction of Y axis.
 * Pins are defined in pinout.h
 *
 * @param debugMode: true to enable serial.print, false otherwise.
 * @param stickMinValue: lowest value of the controller.
 * @param stickMaxValue: highest value of the controller.
 * @param invertYAxis: true if you want to invert Y axis, i.e., if you push forward the stick Y value decreases.
 */
void MotorController::begin(bool debugMode, int stickMinValue, int stickMaxValue, bool invertYAxis) {

    m_debugMode = debugMode;
    m_stickMinValue = stickMinValue;
    m_stickMaxValue = stickMaxValue;
    m_invertYAxis = invertYAxis;
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
 * Moving is from idle position [stickMinValue / 2, stickMaxValue / 2]. Yoy have to call this method when you detect
 * movement in your stick.
 * 
 * ps2 controller example: 
 *      controller interval : [0,255]
 *      idle position       : [128,128]
 *      analogMove(128,   0): Y axis decreases when push forward.
 *      analogMove(128, 255): Y axis increases when push back.
 *      analogMove(255, 128): stick to right.
 *      analogMove(  0, 128): stick to left.
 * 
 *
 * @param x: value of x axis, interval set on begin().
 * @param y: value of y axis, interval set on begin().
 */
void MotorController::analogMove(int x, int y) {

    int diffSteerComputeRange = m_diffSteer.getComputeRange();

    // From PS2 range to DifferentialSteering Range.
    m_diffSteer.computeMotors(
        map(x,
            m_stickMinValue,
            m_stickMaxValue,
            -diffSteerComputeRange,
            diffSteerComputeRange)
        ,
        map(y,
            m_stickMinValue,
            m_stickMaxValue,
            m_invertYAxis ? diffSteerComputeRange : -diffSteerComputeRange,
            m_invertYAxis ? -diffSteerComputeRange : diffSteerComputeRange)
        );

    move(m_diffSteer.computedLeftMotor(), m_diffSteer.computedRightMotor());

    if (m_debugMode) {
        printXY(x, y);
    }
}

/**
 * Move tank with d-pad. Function expects [stickMinValue, stickMaxValue] values, when 0 means button is not pressed.
 *
 * @param padUp: up button value.
 * @param padDown: down button value.
 * @param padLeft: left button value.
 * @param padRight: right button value.
 */
void MotorController::padMove(int padUp, int padDown, int padLeft, int padRight) {

    if (padUp)          move(PAD_FIXED_SPEED, PAD_FIXED_SPEED);
    else if (padDown)   move(-PAD_FIXED_SPEED, -PAD_FIXED_SPEED);
    else if (padLeft)   move(0,  PAD_FIXED_SPEED);
    else if (padRight)  move(PAD_FIXED_SPEED, 0);

    if (m_debugMode) {
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

    int diffSteerComputeRange = m_diffSteer.getComputeRange();

    standby(false);
    digitalWrite(TB_AIN1, (leftMotor  < 0) ? LOW  : HIGH);
    digitalWrite(TB_AIN2, (leftMotor  < 0) ? HIGH : LOW);
    digitalWrite(TB_BIN1, (rightMotor < 0) ? HIGH : LOW);
    digitalWrite(TB_BIN2, (rightMotor < 0) ? LOW  : HIGH);
    int pwml = abs(map(leftMotor,  -diffSteerComputeRange, diffSteerComputeRange, -255, 255));  // [0,255]
    int pwmr = abs(map(rightMotor, -diffSteerComputeRange, diffSteerComputeRange, -255, 255));  // [0,255]
    analogWrite(TB_PWMA, pwml);
    analogWrite(TB_PWMB, pwmr);

    if (m_debugMode) {
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

    if (m_debugMode) {
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

