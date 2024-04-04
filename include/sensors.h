#ifndef SENSORS_H
#define SENSORS_H

#include <ArduinoJson.h>
#include <Arduino.h>
#include <LoRaNode.h>
#include "actionner/HBridge.h"
#include "actionner/PushPullButton.h"
#include "reader/AnalogInputFiltered.h"
#include "reader/DHT/DHT.h"

class Sensors : public LoRaNode
{
public:
    Sensors();
    virtual ~Sensors() = default;

    void appSetup();
    virtual bool appProcessing() override;
    void addJsonTxPayload(JsonDocument& payload);
    virtual bool parseJsonRxPayload(JsonDocument& payload) override;

private:
    void commandRelay(uint8_t relay, uint8_t cmd);
    void parseJsonRelayCmd(uint8_t relay, char* cmd);

    AnalogInputFiltered mLux;
    DHT mTempHumidity;
    PushPullButton mLight;
};

#endif