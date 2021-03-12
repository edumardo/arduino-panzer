#ifndef DRIVEDIRECTION_H
#define DRIVEDIRECTION_H

#include "DCMotorController.h"
#include "DifferentialSteering.h"

class DriveDirection {

    private:
        DCMotorController m_leftMotor;
        DCMotorController m_rightMotor;
        DifferentialSteering m_diffSteer;
        byte m_diffSteerComputeRange;
        static const byte m_pivotYLimit = 32;
        byte m_stickMinValue;
        byte m_stickMaxValue;
        bool m_invertYStick;
        byte m_analogMovePercent;
        byte m_padMovePercent;
        bool m_debugMode;

    public:
        DriveDirection();
        void begin(int pinLeftMotor[4], int pinRightMotor[4], int stickProperties[3]);
        void move(byte stickValueX, byte stickValueY);
        void move(byte padUp, byte padDown, byte padLeft, byte padRight);
        void setMaxVoltagePercentAnalogMove(byte percent);
        void setMaxVoltagePercentPadMove(byte percent);
        void invertYStick();
        void disableMotors();
        void enableDebug();
        void disableDebug();
};

#endif
