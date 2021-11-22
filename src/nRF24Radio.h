#ifndef nRFRadio_h
#define nRFRadio_h

#include "config.h"
#include "Radio.h"
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <DataPacket.h>                     // https://github.com/edumardo/arduino-panzer-tx

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
