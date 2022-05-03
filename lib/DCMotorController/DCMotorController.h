#ifndef DCMOTORCONTROLLER_H
#define DCMOTORCONTROLLER_H

#include <Arduino.h>
#include "RadioStickProperties.h"
#include "DCMotorControllerProperties.h"

class DCMotorController {

    private:
        bool m_debugMode;
        RadioStickProperties m_radioStickProperties;
        DCMotorControllerProperties m_motorProperties;
        uint8_t m_maxVoltagePercent;
        String m_debugName;
        void printDEC(String label, int value);

    public:
        DCMotorController();
        void begin(DCMotorControllerProperties motorControllerProperties, RadioStickProperties radioStickProperties);
        void move(int stickValue);
        void setMaxVoltagePercent(uint8_t percent);
        void enableMotor();
        void disableMotor();
        void enableDebug(String debugName);
        void disableDebug();

};

#endif
