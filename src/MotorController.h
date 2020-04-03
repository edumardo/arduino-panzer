#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

#include "pinout.h"
#include "DifferentialSteering.h"

#define PAD_FIXED_SPEED 90          // 95 is 75% of 127
#define PIVOT_Y_LIMIT   32          // For DifferentialSteering

class MotorController {

    private:
        DifferentialSteering m_diffSteer;
        bool m_debug_mode;
        void move(int computedLeft, int computedRight);

    public:
        MotorController();
        void begin(bool debug_mode);
        /* Inputs [0,255] from ps2*/
        void analogMove(byte XRaw, byte YRaw);
        void padMove(byte PadUp, byte PadDown, byte PadLeft, byte PadRight);
        void standby(bool standby);
};

#endif
