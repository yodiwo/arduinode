// yodiwo_api.h

#ifndef _YODIWO_API_h
#define _YODIWO_API_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "yodiwo_api_defs.h"
#include "yodiwo_client.h"

// these 2 need to be implemented by the api user
void publisher(String msg, String topic);
bool handle_portevent(String portKey, String state);


void set_context(Yodiwo_context *ctx);
void set_nodeinfo(nodeInfoRespStruct &nodeinfo);

bool handle_mqtt_message(char *topic, char *json);

void send_porteventmsg(String portKey, String state);

#endif

