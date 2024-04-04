#ifndef LORAHOMENODE_H
#define LORAHOMENODE_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <LoRaHomeFrame.h>
#include "sensors.h"

class LoRaHomeNode
{
public:
    LoRaHomeNode(Sensors& node);
    void setup();
    void sendToGateway();
    bool receiveLoraMessage();

protected:
    void rxMode();
    void txMode();
    bool receiveAck();
    void send(uint8_t* txBuffer, uint8_t size);
    static uint16_t crc16_ccitt(char* data, unsigned int data_len);
    StaticJsonDocument<LH_FRAME_MAX_PAYLOAD_SIZE> jsonDoc;

    Sensors& mNode;
};

#endif