// 
// 
// 

#include "config.h"
#include "yodiwo_api_defs.h"
#include "yodiwo_api.h"
#include "yodiwo_client.h"
#include "logging.h"
#include <ArduinoJson.h>

Yodiwo_context *client_ctx = NULL;
nodeInfoRespStruct __nodeinfo;


String outgoing_payload;

///////////////////////////////////////////////////////////////////////////
void Yodiwo_serializeThing(Yodiwo_Plegma_Thing_t thing, JsonObject& jthing);
bool deserialize_mqtt_msg(char* content, Yodiwo_Plegma_MqttMsg &mqttMsg);
bool handle_incoming_inner_message(String subtopic, Yodiwo_Plegma_MqttMsg &msg);
bool Yodiwo_handle_activeportkeysmsg(Yodiwo_Plegma_MqttMsg &msg);
bool Yodiwo_handle_nodeinforeq(Yodiwo_Plegma_MqttMsg &msg);
bool Yodiwo_handle_thingsset(Yodiwo_Plegma_MqttMsg &msg);
bool Yodiwo_handle_portstateget(Yodiwo_Plegma_MqttMsg &msg);
bool Yodiwo_handle_portstateset(Yodiwo_Plegma_MqttMsg &msg);
bool Yodiwo_handle_porteventmsg(Yodiwo_Plegma_MqttMsg &msg);
bool Yodiwo_handle_thingsget(Yodiwo_Plegma_MqttMsg &msg);
void Yodiwo_Set_PayloadString(JsonObject& jsonObj);
void Yodiwo_createNodeInfoRsp_Payload();
void Yodiwo_createSetThings_Payload();
void Yodiwo_createGenericResponse_Payload();
void Yodiwo_portStateSet_Response();
void Yodiwo_createMqttMsg(Yodiwo_context &ctx, int flags, int syncId, String payload, String msgType);
void create_porteventmsg(String portKey, String state);
void send_porteventmsg(String portKey, String state);
///////////////////////////////////////////////////////////////////////////

void set_context(Yodiwo_context *ctx)
{
	client_ctx = ctx;
}

void set_nodeinfo(nodeInfoRespStruct &nodeinfo)
{
	__nodeinfo = nodeinfo;
}


bool handle_mqtt_message(char *topic, char *json)
{
	String topicAsString(topic);
	int lastIndex = topicAsString.lastIndexOf('/');
	String actualTopic = topicAsString.substring(lastIndex + 1);
	//Update ctx
	client_ctx->incoming.topic = actualTopic;
	//if (SHOW_DEBUG)
	//{
	//	Serial.println("Topic:" + topicAsString);
	//}
	if (!deserialize_mqtt_msg(json, client_ctx->incoming.mqttMsg))
	{
		Serial.println("JSON parsing, failed");
		return false;
	}
	Serial.println("JSON parsing, success");
	//check the topic
	if (!handle_incoming_inner_message(actualTopic, client_ctx->incoming.mqttMsg))
	{
		Serial.println("Could not create a response.");
		return false;
	}

	return true;
}

bool handle_incoming_inner_message(String subtopic, Yodiwo_Plegma_MqttMsg &msg)
{
	showDebugPerMessage(subtopic, msg);
	if (subtopic == TOPIC_activeportkeysmsg)
	{
		return (Yodiwo_handle_activeportkeysmsg(msg));
	}
	else if (subtopic == TOPIC_nodeinforeq)
	{
		return (Yodiwo_handle_nodeinforeq(msg));
	}
	else if (subtopic == TOPIC_thingsget)
	{
		return (Yodiwo_handle_thingsget(msg));
	}
	else if (subtopic == TOPIC_thingsset)
	{
		return (Yodiwo_handle_thingsset(msg));
	}
	else if (subtopic == TOPIC_portstateget)
	{
		return (Yodiwo_handle_portstateget(msg));
	}
	else if (subtopic == TOPIC_portstateset)
	{
		return (Yodiwo_handle_portstateset(msg));
	}
	else if (subtopic == TOPIC_porteventmsg)
	{
		return (Yodiwo_handle_porteventmsg(msg));
	}
	else
	{
		Serial.print("Got a topic I know nothing about:");
	}
	return true;
}

// =============================================================================================
// Rx msg handlers
// =============================================================================================

/*
ActivePortKeyMsg
*/
bool Yodiwo_handle_activeportkeysmsg(Yodiwo_Plegma_MqttMsg &msg)
{
	Serial.println("ActivePortkeysMsg: not handling it for now");
	return true;
}

/*
nodeInfoReq
*/
bool Yodiwo_handle_nodeinforeq(Yodiwo_Plegma_MqttMsg &msg)
{
	Yodiwo_createNodeInfoRsp_Payload();

	Yodiwo_createMqttMsg(*client_ctx, Response, msg.SyncId, outgoing_payload, TOPIC_nodeinforsp);
	return true;
}

/*
ThingsSet
*/
bool Yodiwo_handle_thingsset(Yodiwo_Plegma_MqttMsg &msg)
{
	Yodiwo_createGenericResponse_Payload();

	Yodiwo_createMqttMsg(*client_ctx, Response, msg.SyncId, outgoing_payload, TOPIC_genericrsp);
	return true;
}

/*
PortStateGet
*/
bool Yodiwo_handle_portstateget(Yodiwo_Plegma_MqttMsg &msg)
{
	Yodiwo_portStateSet_Response();

	Yodiwo_createMqttMsg(*client_ctx, Response, msg.SyncId, outgoing_payload, TOPIC_portstateset);
	return true;
}

/*
PortStateSet
*/
bool Yodiwo_handle_portstateset(Yodiwo_Plegma_MqttMsg &msg)
{
	Yodiwo_createGenericResponse_Payload();

	Yodiwo_createMqttMsg(*client_ctx, Response, msg.SyncId, outgoing_payload, TOPIC_genericrsp);
	return true;
}

/*
PortEventMsg
*/
bool Yodiwo_handle_porteventmsg(Yodiwo_Plegma_MqttMsg &msg)
{
	StaticJsonBuffer<1024> jsonBuffer;
	JsonObject& rootPortEventMsg = jsonBuffer.parseObject(msg.Payload.c_str());
	Serial.println("Got PortEventMsg");

	if (!rootPortEventMsg.success())
	{
		Serial.println("JSON parsing for porteventmsg failed!");
		return false;
	}
	//if (SHOW_DEBUG)
	//{
	//	  rootPortEventMsg.prettyPrintTo(Serial);
	//}
	JsonArray& rootPortEventMsgPorts = rootPortEventMsg["PortEvents"].asArray();
	JsonObject& portObj = rootPortEventMsgPorts.get(0);
	handle_portevent(portObj["PortKey"].asString(), portObj["State"].asString());
	return true;
}


/*
ThingsGet
*/
bool Yodiwo_handle_thingsget(Yodiwo_Plegma_MqttMsg &msg)
{
	Yodiwo_createSetThings_Payload();
	Yodiwo_createMqttMsg(*client_ctx, Response, msg.SyncId, outgoing_payload, TOPIC_thingsset);
	return true;
}


// =============================================================================================
// Create payloads
// =============================================================================================
/*
Payload for all messages
*/

void Yodiwo_Set_PayloadString(JsonObject& jsonObj)
{
	outgoing_payload = "";
	jsonObj.printTo(outgoing_payload);
}

/*
NodeInfoRsp
*/
void Yodiwo_createNodeInfoRsp_Payload()
{
	StaticJsonBuffer<1024> jsonPayloadBuffer;
	JsonObject& rootnodeInfoResp = jsonPayloadBuffer.createObject();
	rootnodeInfoResp["Name"] = __nodeinfo.Name;
	rootnodeInfoResp["Type"] = __nodeinfo.Type;
	rootnodeInfoResp["Capabilities"] = __nodeinfo.Capabilities;
	JsonArray& thingTypes = rootnodeInfoResp.createNestedArray("ThingTypes");
	//JsonObject& thingTypesObj = thingTypes.createNestedObject();
	//{
	//	thingTypesObj["Type"] = "Arduino";
	//	thingTypesObj["Searchable"] = true;
	//	thingTypesObj["Description"] = "some description";
	//}
	rootnodeInfoResp["ThingsRevNum"] = 0;// nodeInfoResp.ThingsRevNum;
	rootnodeInfoResp["SupportedApiRev"] = __nodeinfo.SupportedApiRev;
	rootnodeInfoResp["BlockLibraries"] = "";
	rootnodeInfoResp["SeqNo"] = 0;// nodeInfoResp.SeqNo;
								  // Step 3: Generate the JSON string
	if (SHOW_DEBUG)
		rootnodeInfoResp.printTo(Serial);
	Yodiwo_Set_PayloadString(rootnodeInfoResp);
}

/*
ThingsSet
*/
void Yodiwo_createSetThings_Payload()
{
	StaticJsonBuffer<1536> jsonPayloadBuffer;
	JsonObject& jobj = jsonPayloadBuffer.createObject();
	jobj["Operation"] = 1; //payloadRoot["Operation"]; //gepa speaking
	jobj["Status"] = true; //lets assume handling was ok
	jobj["RevNum"] = 0;//payloadRoot["RevNum"];
	jobj["SeqNo"] = 0;//payloadRoot["SeqNo"];
	JsonArray& data = jobj.createNestedArray("Data");
	int i;
	for (i = 0; i < client_ctx->thingsNum; i++) {
		JsonObject& jthing = data.createNestedObject();
		Yodiwo_serializeThing(client_ctx->_things[i], jthing);
	}

	jobj.printTo(Serial);
	Serial.println();
	Yodiwo_Set_PayloadString(jobj);
}

/*
GenericRsp
*/
void Yodiwo_createGenericResponse_Payload()
{
	StaticJsonBuffer<512> jsonPayloadBuffer;
	JsonObject& jobj = jsonPayloadBuffer.createObject();
	jobj["IsSuccess"] = true;
	jobj["StatusCode"] = 0;
	jobj["Message"] = "";
	jobj["SeqNo"] = 0;// payloadRoot["SeqNo"];
					  // Step 3: Generate the JSON string
	if (SHOW_DEBUG)
		jobj.printTo(Serial);
	Yodiwo_Set_PayloadString(jobj);
}

/*
PortStateSet
*/
void Yodiwo_portStateSet_Response()
{
	Serial.println("\n\n\nStart preparing the response (portstateset):");
	StaticJsonBuffer<1024> jsonPayloadBuffer;
	JsonObject& jobj = jsonPayloadBuffer.createObject();
	jobj["Operation"] = (int)Yodiwo_ePortStateOperation_AllPortStates;
	//things to be added in this line TODO@
	JsonArray& thingPortsStateArray = jobj.createNestedArray("PortStates");
	JsonObject& thingPortState = thingPortsStateArray.createNestedObject();
	{
		thingPortState["State"] = false;
		thingPortState["RevNum"] = 0;//1;
	}
	jobj["SeqNo"] = 0;//payloadRoot["SeqNo"];

	if (SHOW_DEBUG)
		jobj.printTo(Serial);
	Yodiwo_Set_PayloadString(jobj);
}

// =============================================================================================
// Create MQTT msg
// =============================================================================================

void Yodiwo_createMqttMsg(Yodiwo_context &ctx, int flags, int syncId, String payload, String msgType)
{
	String topic_to_publish_to = "/api/in/1/";
	StaticJsonBuffer<2048> jsonMqttBuffer;
	JsonObject& rootOut = jsonMqttBuffer.createObject();
	rootOut["Flags"] = flags;
	rootOut["SyncId"] = syncId;
	rootOut["Payload"] = payload;
	rootOut["PayloadSize"] = payload.length();
	Serial.println();
	if (SHOW_DEBUG)
	{
		rootOut.prettyPrintTo(Serial);
		Serial.println();
	}
	topic_to_publish_to += ctx.UserKey;
	topic_to_publish_to += "/";
	topic_to_publish_to += ctx.NodeKey;
	topic_to_publish_to += "/";
	topic_to_publish_to += msgType;
	String mqtt_msg;
	rootOut.printTo(mqtt_msg);
	publisher(mqtt_msg, topic_to_publish_to);
}


// =============================================================================================
// Send PortEventMsg
// =============================================================================================


//TODO: multiple portevents
void create_porteventmsg(String portKey, String state)
{
	StaticJsonBuffer<1024> jsonPayloadBuffer;
	JsonObject& jobj = jsonPayloadBuffer.createObject();
	JsonArray& portEvents = jobj.createNestedArray("PortEvents");
	JsonObject& portEvent = portEvents.createNestedObject();
	{
		String stateString = String(state);

		portEvent["PortKey"] = portKey;
		portEvent["State"] = state;
		portEvent["RevNum"] = 0;//1;
		portEvent["Timestamp"] = 0;//1;
	}

	jobj["SeqNo"] = 0;//payloadRoot["SeqNo"];p

	portEvent.printTo(Serial);
	Yodiwo_Set_PayloadString(jobj);
}



void send_porteventmsg(String portKey, String state)
{
	create_porteventmsg(portKey, state);

	Yodiwo_createMqttMsg(*client_ctx, None, 0, outgoing_payload, TOPIC_porteventmsg);
}


// =============================================================================================
// Serialization
// =============================================================================================

//JsonArray& thingsRootArray = jsonBuffer.createArray();
void Yodiwo_serializeThing(Yodiwo_Plegma_Thing_t thing, JsonObject& jthing)
{
	jthing["ThingKey"] = thing.ThingKey;
	jthing["Name"] = thing.Name;
	JsonArray& thingConfig = jthing.createNestedArray("Config");
	JsonArray& thingReadOnlyInfo = jthing.createNestedArray("ReadonlyInfo");
	JsonArray& thingPortsArray = jthing.createNestedArray("Ports");
	JsonObject& thingPort = thingPortsArray.createNestedObject();
	{
		thingPort["ConfFlags"] = (int)thing.Ports.elems[0].ConfFlags;
		thingPort["Description"] = thing.Ports.elems[0].Description;
		thingPort["Name"] = thing.Ports.elems[0].Name;
		thingPort["Type"] = (int)thing.Ports.elems[0].Type;
		thingPort["State"] = thing.Ports.elems[0].State;
		thingPort["RevNum"] = thing.Ports.elems[0].RevNum;
		thingPort["ioDirection"] = (int)thing.Ports.elems[0].ioDirection;
		thingPort["PortKey"] = thing.Ports.elems[0].PortKey;
		thingPort["Semantics"] = thing.Ports.elems[0].Semantics;
	}

	jthing["Type"] = thing.Type;
	jthing["BlockType"] = thing.BlockType;
	JsonObject& thingUIHints = jthing.createNestedObject("UIHints");
	thingUIHints["Description"] = thing.UIHints.Description;
	thingUIHints["IconURI"] = thing.UIHints.IconURI;
}

bool deserialize_mqtt_msg(char* content, Yodiwo_Plegma_MqttMsg &mqttMsg)
{
	StaticJsonBuffer<2048> incoming_json_buffer;
	JsonObject& rootMqttMsg = incoming_json_buffer.parseObject(content);
	//reset ctx
	mqttMsg.Flags = 0;
	mqttMsg.SyncId = 0;
	mqttMsg.Payload = "\0";
	mqttMsg.PayloadSize = 0;
	if (!rootMqttMsg.success())
	{
		Serial.println("JSON parsing failed!");
		return false;
	}
	if (SHOW_DEBUG)
	{
		rootMqttMsg.prettyPrintTo(Serial);
	}
	//Update ctx
	mqttMsg.Flags = rootMqttMsg[MQTT_MSG_FLAGS];
	mqttMsg.SyncId = rootMqttMsg[MQTT_MSG_SYNC_ID];
	mqttMsg.Payload = rootMqttMsg[MQTT_MSG_PAYLOAD].as<String>();
	mqttMsg.PayloadSize = rootMqttMsg[MQTT_MSG_PAYLOAD_SIZE];
	return true;
}
