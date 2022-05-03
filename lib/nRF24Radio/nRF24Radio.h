#ifndef nRFRadio_h
#define nRFRadio_h

#include "config.h"
#include "Radio.h"
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
//#include <DataPacket.h>                     // https://github.com/edumardo/arduino-panzer-tx

struct DataPacket {
    uint16_t lx;        //  2
    uint16_t ly;        //  2
    bool     lb;        //  1
    uint16_t rx;        //  2
    uint16_t ry;        //  2
    bool     rb;        //  1
};

class nRF24Radio : public Radio {

    private:
        DataPacket m_dataPacket;
        RF24 m_radio;
    
    public:
        nRF24Radio();
        void begin(RadioStickProperties radioStickProperties);
        bool isOnline();
        void read();
        bool button(uint16_t button);
        void printDebug();
};

#endif
