#ifndef GUN_H
#define GUN_H

#include <Arduino.h>

class Gun {

    protected:
        bool m_debugMode;
        static bool m_readyToFire;

    public:
        Gun();
        virtual void fire() = 0;
        bool readyToFire();
        void toggleDebugMode(bool debugMode); 
        void printDebug();

};

#endif
