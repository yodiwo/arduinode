#ifndef __CONFIG_H__
#define __CONFIG_H__

// =============================================================================================
// User Configuration
// =============================================================================================
#define WIFI_SSID "<SSID>"
#define WIFI_PASSWORD "<PASSWORD>"

#define USERKEY "<USERKEY>"
#define NODEKEY "<NODEKEY>"
#define NODE_SECRET "<NODESECRET>"

#define SHOW_DEBUG 1

// =============================================================================================
// other hardcoded values
// =============================================================================================

#define SUPPORTED_API_VER 1
#define SHOW_DEBUG 1
#define THING_NUM 3
#define REPEAT_EVERY 2000
#define BUILTIN_LED_PIN 16
#define TEST_PIN 0


// =============================================================================================
// Other configuration (should not need tampering with)
// =============================================================================================

#define MQTT_SERVER "cyan.yodiwo.com"
#define USE_SSL 1

#ifdef USE_SSL
#define MQTT_PORT 8883
#else
#define MQTT_PORT 1883
#endif

#endif //__CONFIG_H__
