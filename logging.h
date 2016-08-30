#ifndef __LOGGING_H__
#define __LOGGING_H__

#include "yodiwo_client.h"

void showContext(Yodiwo_context &ctx);

void showContextIncoming(Yodiwo_context &ctx);

void showDebugPerMessage(Yodiwo_API_msg &msg);
void showDebugPerMessage(String topic, Yodiwo_Plegma_MqttMsg &msg);

#endif // __LOGGING_H__
