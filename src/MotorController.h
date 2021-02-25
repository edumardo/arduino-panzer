#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

#include "pinout.h"
#include "DifferentialSteering.h"

class MotorController {

    private:
        const int PAD_FIXED_SPEED = 90;     // 90 is 75% of 127
        const int PIVOT_Y_LIMIT = 32;       // For DifferentialSteering
        DifferentialSteering m_diffSteer;
        bool m_debugMode;
        int m_stickMinValue;
        int m_stickMaxValue;
        bool m_invertYAxis;
        void move(int leftMotor, int rightMotor);
        void printXY(int x, int y);

    public:
        MotorController();
        void begin(bool debugMode, int stickMinValue, int stickMaxValue, bool invertYAxis);
        void analogMove(int x, int y);
        void padMove(int padUp, int padDown, int padLeft, int padRight);
        void standby(bool standby);
};

#endif
