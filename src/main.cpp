#include <Arduino.h>
#include <ArduinoJson.h>
#include <LoRa.h>
#include <loRaOverlay/LoRaHomeNode.h>
#include <SPI.h>
#include "sensors.h"

#define DEBUG

#ifdef DEBUG
#define DEBUG_MSG(x) Serial.println(F(x))
#define DEBUG_MSG_VAR(x) Serial.println(x)
#define DEBUG_MSG_ONE_LN(x) Serial.print(x)
unsigned int i;
#else
#define DEBUG_MSG(x) // define empty, so macro does nothing
#endif

// Objects instentiation
Sensors mNode;
LoRaHomeNode mLoRaHome(mNode.getNodeId());

// sampling management
unsigned long nextSendTime;         // Next send time
unsigned long nextProcessTime;      // Next processing time
unsigned long nextSendRetryTime;    // Next send retry time
bool forceProcessing;
JsonDocument receivedMessage;

void setup()
{

  //initialize Serial Monitor
#ifdef DEBUG
  Serial.begin(115200);
  while (!Serial)
    ;
#endif
  DEBUG_MSG("initializing LoRa Node");
  // initialize LoRa    
  mLoRaHome.setup();
  // call node specific configuration (end user)
  mNode.appSetup();

  // Update Data before start
  forceProcessing = true;
}

/**
* Main loop of the LoRa Node
* Constantly try to receive JSON LoRa message
* Every transmissionTimeInterval send JSON LoRa messages
*/
void loop()
{
  unsigned long tick = millis();

#ifdef DEBUG
  if(1000 < i){
    DEBUG_MSG_ONE_LN(".");
    i = 0;
  }
  i++;
#endif

  // Application processing Task
  if (tick  >= nextProcessTime
    || forceProcessing) {

    forceProcessing = mNode.appProcessing();

    nextProcessTime = millis() + mNode.getProcessingTimeInterval();
  }

  // Send Task
  if ((tick >= nextSendTime)
    || (mNode.getTransmissionNowFlag() == true)) {

    DEBUG_MSG("Send Task");
    mNode.setTransmissionNowFlag(false);
    if( true == mLoRaHome.sendToGateway(mNode.getJsonTxPayload(), mNode.getTxCounter())) {
      mNode.incrementTxCounter();
    }

    nextSendTime = millis() + mNode.getTransmissionTimeInterval();
    nextSendRetryTime = millis() + mLoRaHome.getRetrySendMessageInterval();
  }

  // Retry message not received by the gateway
  if ((tick >= nextSendRetryTime) && (mLoRaHome.isWaitingForAck())) {
    DEBUG_MSG("Retry Send Task");
    mLoRaHome.retrySendToGateway();

    nextSendRetryTime = millis() + mLoRaHome.getRetrySendMessageInterval();
  }

  // Receive Task
  bool messageReceived = mLoRaHome.receiveLoraMessage(receivedMessage);
  if(messageReceived){
    DEBUG_MSG("Receive Task");
    mNode.parseJsonRxPayload(receivedMessage);
    forceProcessing = true;
  }
}
