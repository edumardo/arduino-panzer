#ifndef DRIVEDIRECTION_H
#define DRIVEDIRECTION_H

#include "DCMotorController.h"
#include <DifferentialSteering.h>
#include "RadioStickProperties.h"
#include "DCMotorControllerProperties.h"

class DriveDirection {

    private:
        DCMotorController m_leftMotor;
        DCMotorController m_rightMotor;
        DifferentialSteering m_diffSteer;
        byte m_diffSteerComputeRange;
        static const byte m_pivotYLimit = 32;
        RadioStickProperties m_radioStickProperties;
        byte m_analogMovePercent;
        byte m_padMovePercent;
        bool m_debugMode;

    public:
        DriveDirection();
        void begin(DCMotorControllerProperties leftMotorConfig, DCMotorControllerProperties rightMotorConfig, RadioStickProperties radioStickProperties);
        void move(byte stickValueX, byte stickValueY);
        void move(byte padUp, byte padDown, byte padLeft, byte padRight);
        void setMaxVoltagePercentAnalogMove(byte percent);
        void setMaxVoltagePercentPadMove(byte percent);
        void disableMotors();
        void enableDebug();
        void disableDebug();
};

#endif
