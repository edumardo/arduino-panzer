#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

#include "pinout.h"
#include "DifferentialSteering.h"

#define PAD_FIXED_SPEED 90          // 95 is 75% of 127
#define PIVOT_Y_LIMIT   32          // For DifferentialSteering
#define PS2_MAX_VALUE   255         // analog stick and d-pad max value
#define PS2_MIN_VALUE   0           // analog stick and d-pad min value

class MotorController {

    private:
        DifferentialSteering m_diffSteer;
        bool m_debugMode;
        void move(int leftMotor, int rightMotor);
        void printXY(int x, int y);

    public:
        MotorController();
        void begin(bool debugMode);
        void analogMove(int x, int y);
        void padMove(int padUp, int padDown, int padLeft, int padRight);
        void standby(bool standby);
};

#endif
