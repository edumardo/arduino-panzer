#include "nRF24Radio.h"

nRF24Radio::nRF24Radio() //@TODO
    : Radio(), RF24(9, 10) {

}

void nRF24Radio::begin(RadioStickProperties radioStickProperties) {

    const uint64_t pipe = 0xE8E8F0F0E1LL; //@TODO

    m_stickProperties = radioStickProperties;
    m_radio.begin();
    m_radio.openReadingPipe(1, pipe);
    m_radio.startListening();
}

void nRF24Radio::read() {

    
    if ((millis() - m_msLastRead > NRF24_MS_BETWEEN_READS) && (m_radio.available())){
        m_msLastRead = millis();
        m_radio.read(m_dataPacket, sizeof(DataPacket));
        // @TODO
    }
}

bool nRF24Radio::button(uint16_t button) {

    // @TODO
    return true;
}

void nRF24Radio::printDebug(){

    //@TODO
}

bool PS2Radio::isOnline() {

    return m_radio.available();
}