#ifndef NODE_CONFIG_H
#define NODE_CONFIG_H

const uint8_t NODE_ID = 3;
const unsigned long PROCESSING_TIME_INTERVAL   = 600000;  // 10 mins
const unsigned long TRANSMISSION_TIME_INTERVAL = 300000; // 5 mins
const unsigned long BUTTON_UP_TIME = 1000;  // 1 sec
const uint16_t MY_NETWORK_ID = 0xACDC;

// Jeedom Message
#define MSG_LUX "lux"
#define MSG_TEMP "temp"
#define MSG_HUMIDITY "hum"
#define MSG_LIGHT_STATE "light_state"

// Jeedom Command
#define MSG_LIGHT "light"
#define MSG_SAMPLING "sampling"
#define ON "on"
#define OFF "off"

#define PIN_LUX_METER A0
#define PIN_DHT22 A2
#define PIN_LIGHT 2
#endif 