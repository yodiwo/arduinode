#include "config.h"
#include "logging.h"
#include <ESP8266WiFi.h>


// =============================================================================================
// Logging
// =============================================================================================

using namespace std;

void showContext(Yodiwo_context &ctx)
{
	showContextIncoming(ctx);
	//showContextOutgoing();
}

void showContextIncoming(Yodiwo_context &ctx)
{
	Serial.println();
	Serial.println("Showing ctx:");
	Serial.printf("Incoming/Topic: %s", ctx.incoming.topic.c_str());
	Serial.printf("Incoming/Mqtt/Flags: %d", ctx.incoming.mqttMsg.Flags);
	Serial.printf("Incoming/Mqtt/SyncId: %d", ctx.incoming.mqttMsg.SyncId);
	Serial.printf("Incoming/Mqtt/Payload: %s", ctx.incoming.mqttMsg.Payload.c_str());
	Serial.printf("Incoming/Mqtt/PayloadSize: %d", ctx.incoming.mqttMsg.PayloadSize);
}

void showDebugPerMessage(Yodiwo_API_msg &msg)
{
	showDebugPerMessage(msg.topic, msg.mqttMsg);
}

void showDebugPerMessage(String topic, Yodiwo_Plegma_MqttMsg &msg)
{
	String payloadAsString = msg.Payload;
	if (SHOW_DEBUG)
	{
		Serial.println();
		Serial.print("Got message for topic:");
		Serial.println(topic);
	}
	if (SHOW_DEBUG)
	{
		Serial.println("\n\n\nPAYLOAD:");
		Serial.print("[");
		Serial.println(payloadAsString);
		Serial.print("]\n");
	}
}

