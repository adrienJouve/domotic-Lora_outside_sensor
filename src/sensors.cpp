#include "sensors.h"
#include "NodeConfig.h"

#define DEBUG
#ifdef DEBUG
#define DEBUG_MSG(x) Serial.println(F(x))
#define DEBUG_MSG_VAR(x) Serial.println(x)
#else
#define DEBUG_MSG(x) // define empty, so macro does nothing
#define DEBUG_MSG_VAR(x)
#endif


/*!********************************************************************
 * @brief Construct a new Node:: Node object
 **********************************************************************/
Sensors::Sensors() :
    LoRaNode(NODE_ID, TRANSMISSION_TIME_INTERVAL, PROCESSING_TIME_INTERVAL, true),
    mLux(PIN_LUX_METER),
    mTempHumidity(PIN_DHT22, DHT22),
    mLight(PIN_LIGHT, BUTTON_UP_TIME)
{
}

/**
* Function invoked by the node right after its own setup (as per Arduino Setup function)
* To be used for applicative setup
*/
void Sensors::appSetup() {

    //Initialize Temperature Humidity sensor
    mTempHumidity.begin();
}


/**
* Add JSON Tx payload messages
* @param payload the JSON payload to be completed as per application needs
*/
JsonDocument Sensors::getJsonTxPayload() {
    JsonDocument payload;
    payload[MSG_LUX] = mLux.Get();
    // DEBUG_MSG("Lux: ");
    // DEBUG_MSG_VAR(mLux.Get());
    payload[MSG_HUMIDITY] = mTempHumidity.readHumidity();
    // DEBUG_MSG("Humidity: ");
    // DEBUG_MSG_VAR(mTempHumidity.readHumidity());
    payload[MSG_TEMP] = mTempHumidity.readTemperature();
    // DEBUG_MSG("Temperature: ");
    // DEBUG_MSG_VAR(mTempHumidity.readTemperature());
    return payload;
}

/**
* Parse JSON Rx payload
* One should avoid any long processing in this routine. LoraNode::AppProcessing is the one to be used for this purpose
* Limit the processing to parsing the payload and retrieving the expected attributes
* @param payload the JSON payload received by the node
* return true in case of new message received
*/
bool Sensors::parseJsonRxPayload(JsonDocument& payload) {
    bool isMessageReceived(false);

    if (!payload[MSG_LIGHT].isNull()) {
        if (ON == payload[MSG_LIGHT]) {
            mLight.Enable();
        }
        isMessageReceived = true;
    }
    if (!payload[MSG_SAMPLING].isNull()) {
        setTransmissionTimeInterval(payload[MSG_SAMPLING]);
    }
    return isMessageReceived;
}

/**
* App processing of the node.
* Invoke every loop of the nodes before Rx and Tx
* ONe should benefit from using processingTimeInterval to avoid overloading the node
*/
bool Sensors::appProcessing() {
    bool isRunFastly(false);

    // Sample lux measure
    mLux.Run();

    // Handle push pull button to go back to off
    if(eActive == mLight.GetState()) {
        mLight.Handle();
        isRunFastly = true;
    }

    return isRunFastly;
}