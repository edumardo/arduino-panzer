#ifndef PS2RADIO_H
#define PS2RADIO_H

#include <Arduino.h>
#include "config.h"
#include "Radio.h"
#include "PS2X_lib.h"

class PS2Radio: public Radio {

    private:
        PS2X m_ps2x;
        byte m_ps2xConfigError;

    public:
        PS2Radio();
        void begin(RadioStickProperties radioStickProperties);
        bool isOnline();
        void read();
        bool button(uint16_t button);
        void printDebug();
};

#endif
