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
        bool m_debug_mode;
        void move(int leftMotor, int rightMotor);
        void printXY(byte x, byte y);

    public:
        MotorController();
        void begin(bool debug_mode);
        /* Inputs [0,255] from ps2*/
        void analogMove(byte x, byte y);
        void padMove(byte padUp, byte padDown, byte padLeft, byte padRight);
        void standby(bool standby);
};

#endif
