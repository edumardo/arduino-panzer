#ifndef DCMOTORCONTROLLERPROPERTIES_H
#define DCMOTORCONTROLLERPROPERTIES_H

#include <Arduino.h>

class DCMotorControllerProperties {
    public:
        uint16_t pwmPrescaler;
        uint8_t  pwmPin;
        uint8_t  in1Pin;
        uint8_t  in2Pin;
        uint8_t  standbyPin;
};

#endif
