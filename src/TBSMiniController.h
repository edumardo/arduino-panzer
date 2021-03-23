#ifndef TBSMINICONTROLLER_H
#define TBSMINICONTROLLER_H

#include <Servo.h>
#include <Arduino.h>

class TBSMiniController {

    private:
        // Prop1 - Throttle speed
        static const int m_PROP1_IDLE        = 1500;                // Idle throttle
        static const int m_PROP1_FULL_SPEED  = 2000;                // Full speed

        // Prop2 - 2 sounds in direct control mode, you must emulate a 3-position switch
        static const int m_PROP2_SWITCH_OFF = 1500;                // Off position for Prop2 Function 1/2 (default)
        static const int m_PROP2_SWITCH_1   = 2000;                // Prop2, sound for 2nd Coder Position 1 (We use it for engine startup/shutdown)
        static const int m_PROP2_SWITCH_2   = 1000;                // Prop2, sound for 2nd Coder Position 2 (We use it for repair sound)

        byte m_prop1Pin;
        byte m_prop2Pin;
        Servo m_prop1;
        Servo m_prop2;
        bool m_isEngineRunning;
        int m_minSpeed;
        int m_maxSpeed;
        int m_currentSpeed;                             // In microseconds
        bool m_debugMode;

        void printDebug();

    public:
        TBSMiniController();
        void begin(int prop1Pin, int prop2Pin, int minSpeed, int maxSpeed);
        void startEngine();
        void stopEngine();
        void idleEngine();
        void setEngineSpeed(int speed);
        bool isEngingeRunning();
        void enableDebug();
        void disableDebug();

        void playTurretRotation();
        void stopTurretRotation();

        void playCannon();
        void stopCannon();
};

#endif
