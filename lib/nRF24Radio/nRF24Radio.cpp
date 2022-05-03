#include "nRF24Radio.h"
#include "Utilities.h"

nRF24Radio::nRF24Radio()
    : Radio(), m_radio(NRF24_CE, NRF24_CSN)
{
}

void nRF24Radio::begin(RadioStickProperties radioStickProperties)
{
    uint8_t RF24_ADDRESS[6] = "AP-TX";

    m_stickProperties = radioStickProperties;
    m_radio.begin();
    m_radio.openReadingPipe(1, RF24_ADDRESS);
    m_radio.startListening();

    resetData();
}

void nRF24Radio::read()
{
    unsigned long msCurrentTime = millis();
    unsigned long msSinceLastRead = msCurrentTime - m_msLastRead;

    if (msSinceLastRead > NRF24_MS_MAX_TIME_BETWEEN_READS)
    {
        resetData();
    }

    if ((msSinceLastRead > NRF24_MS_BETWEEN_READS) && (m_radio.available()))
    {
        m_msLastRead = msCurrentTime;
        m_radio.read(&m_dataPacket, sizeof(DataPacket));

        m_steeringThrottle = m_dataPacket.ly;
        m_steeringTurn     = m_dataPacket.lx;
        m_turretRotation   = m_dataPacket.rx;
        m_gunElevation     = m_dataPacket.ry;
    }

    printDebug();
}

bool nRF24Radio::button(uint16_t button)
{
    // @TODO
    return true;
}

void nRF24Radio::printDebug()
{
    Serial.print("[");
    Serial.print("LA");
    Serial.print(" ");
    Serial.print(m_steeringThrottle);
    Serial.print(" | ");
    Serial.print(m_steeringTurn);
    Serial.print("]");

    Serial.print("[");
    Serial.print("RA");
    Serial.print(" ");
    Serial.print(m_turretRotation);
    Serial.print(" | ");
    Serial.print(m_gunElevation);
    Serial.print("]");
    Serial.println();
}

bool nRF24Radio::isOnline()
{
    return m_radio.available();
}