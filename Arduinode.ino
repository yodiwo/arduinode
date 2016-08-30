#include "config.h"
#include <ESP8266WiFi.h>
#include "yodiwo_api_defs.h"
#include "yodiwo_api.h"
#include "logging.h"
#include "yodiwo_client.h"
#include "PubSubClient.h"
#include <ArduinoJson.h>


// =============================================================================================
// Function declarations
// =============================================================================================

void msgReceivedCallback(char* topic, byte* payload, unsigned int length);


// =============================================================================================
// Variables
// =============================================================================================

#ifdef USE_SSL
WiFiClientSecure espClient;
#else
WiFiClient espClient;
#endif

PubSubClient client(MQTT_SERVER, MQTT_PORT, msgReceivedCallback, espClient);
nodeInfoRespStruct nodeInfoResp;
Yodiwo_context ctx;

//Global Thing
Yodiwo_Plegma_Thing_t *led_thing;

Yodiwo_Plegma_Thing_t *button_thing;


// json -related
StaticJsonBuffer<512> jsonThingBuffer;
JsonObject& thingRoot = jsonThingBuffer.createObject();
StaticJsonBuffer<512> jsonThing2Buffer;
JsonObject& thing2Root = jsonThing2Buffer.createObject();

bool tmpState = true;
bool Yodiwo_send_flag = false;

int led_state = LOW;

// =============================================================================================
// Arduino starter
// =============================================================================================

void loop() {
	if (!client.connected()) {
		Serial.println("Disconnected, reconnecting...");
		reconnect();
	}
	client.loop();

	if (Yodiwo_send_flag)
	{
		tmpState = !tmpState;
		bool state = !(bool)digitalRead(TEST_PIN);
		Serial.printf("new state is %d", (int)state);
		send_button_event(state);
		Yodiwo_send_flag = false;
	}
}

void setup()
{
	pinMode(BUILTIN_LED_PIN, OUTPUT);     // Initialize the BUILTIN_LED pin as an output

	Serial.begin(115200);
	pinMode(TEST_PIN, INPUT);
	digitalWrite(TEST_PIN, HIGH);
	attachInterrupt(digitalPinToInterrupt(TEST_PIN), irq_handler, CHANGE);
	Yodiwo_init();
	setup_wifi();
	reconnect();
}

void irq_handler()
{
	Yodiwo_send_flag = true;
	Serial.println("Interrupt");
}

// =============================================================================================
// WiFi setup
// =============================================================================================

void setup_wifi()
{
	delay(10);
	// We start by connecting to a WiFi network
	Serial.println();
	Serial.println();

	Serial.printf("Connecting to %s", WIFI_SSID);

	WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}

	Serial.println();
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());
}

// =============================================================================================
// Initialize Yodiwo context
// =============================================================================================

void Yodiwo_init()
{
	//init context
	initContext();
	//init thing struct
	make_nodeinfo();
	//initialize thing structures
	initThingsStruct();

	//set the data on the yodiwo library
	set_context(&ctx);
	set_nodeinfo(nodeInfoResp);
}
/*
  Since pairing is not supported at the moment setup the correct credentials here.
*/
void initContext()
{
	ctx.UserKey = USERKEY;
	ctx.NodeKey = NODEKEY;
	ctx.NodeSecretKey = NODE_SECRET;
	ctx.LedThingKey = ctx.NodeKey + "-Blue_LED";
	ctx.LedThingPortKey = ctx.LedThingKey + "-Port1";
	ctx.ButtonThingKey = ctx.NodeKey + "-Button";
	ctx.ButtonThingPortKey = ctx.ButtonThingKey + "-Port1";
}

void make_nodeinfo()
{
	nodeInfoResp.Name = "ArduinoNode";
	nodeInfoResp.Type = TestEndpointType;
	nodeInfoResp.Capabilities = NoneCapabilities;
	nodeInfoResp.ThingTypes;
	nodeInfoResp.ThingsRevNum = 0;
	nodeInfoResp.SupportedApiRev = SUPPORTED_API_VER;
	nodeInfoResp.BlockLibraries = NULL;
	nodeInfoResp.SeqNo = 0;//payloadRoot[JSON_MEMBER_PAYLOAD_SEQ_NO];
}



// =============================================================================================
// Reconnect
// =============================================================================================

void reconnect()
{
	// Loop until we're reconnected
	while (!client.connected())
	{
		Serial.print("Attempting MQTT connection...");
		// Attempt to connect
		if (client.connect(ctx.NodeKey.c_str(), ctx.NodeKey.c_str(), ctx.NodeSecretKey.c_str()))
		{
			digitalWrite(BUILTIN_LED_PIN, led_state);   // Turn the LED on by setting to LOW (Note that LOW is the voltage level
			Serial.println("connected");
			String topic_to_subscribe_to = "/api/out/1/";
			topic_to_subscribe_to += ctx.NodeKey;
			topic_to_subscribe_to += "/#";
			client.subscribe(topic_to_subscribe_to.c_str());
		}
		else
		{
			Serial.print("failed, rc=");
			Serial.print(client.state());
			Serial.println(" try again in 5 seconds");
			// Wait 5 seconds before retrying
			delay(5000);
		}
	}
}

// =============================================================================================
// Client Callback
// =============================================================================================

void msgReceivedCallback(char* topic, byte* payload, unsigned int length)
{
	if (SHOW_DEBUG)
		Serial.printf("Got message on topic: %s\n", topic);
	//call yodiwo message handling function
	handle_mqtt_message(topic, (char *)payload);
}

// =============================================================================================
// Arduino Things
// =============================================================================================

void initThingsStruct()
{
	led_thing = &ctx._things[0];
	button_thing = &ctx._things[1];

	init_led_thing(*led_thing, ctx.LedThingKey);
	init_button_thing(*button_thing, ctx.ButtonThingKey);
	ctx.thingsNum = 2;
}

void init_led_thing(Yodiwo_Plegma_Thing &thing, String thingKey)
{
	Yodiwo_Plegma_Port_t *port = new Yodiwo_Plegma_Port_t();

	port->ConfFlags = Yodiwo_ePortConf_IsTrigger;
	port->Description = "little blue light";
	port->Name = "led";
	port->Type = Yodiwo_ePortType_Integer;
	port->State = "";
	port->RevNum = 0;
	port->ioDirection = Yodiwo_ioPortDirection_Input;
	port->PortKey = thingKey + "-Port1";
	port->Semantics = "";

	thing.ThingKey = thingKey;
	thing.Name = "LED";
	thing.Config.num = 0;
	thing.Config.elems = NULL;
	thing.Ports.num = 1;
	thing.Ports.elems = port;
	thing.Type = "";
	thing.BlockType = "";
	thing.UIHints.Description = "";
	thing.UIHints.IconURI = "/Content/img/icons/Generic/thing-genericlight.png";
}

void init_button_thing(Yodiwo_Plegma_Thing &thing, String thingKey)
{
	Yodiwo_Plegma_Port_t *port = new Yodiwo_Plegma_Port_t();

	port->ConfFlags = Yodiwo_ePortConf_IsTrigger;
	port->Description = "Button";
	port->Name = "button";
	port->Type = Yodiwo_ePortType_Integer;
	port->State = "";
	port->RevNum = 0;
	port->ioDirection = Yodiwo_ioPortDirection_Output;
	port->PortKey = (char *)ctx.ButtonThingPortKey.c_str();
	port->Semantics = "";

	thing.ThingKey = (char *)ctx.ButtonThingKey.c_str();
	thing.Name = "Button";
	thing.Config.num = 0;
	thing.Config.elems = NULL;
	thing.Ports.num = 1;
	thing.Ports.elems = port;
	thing.Type = "";
	thing.BlockType = "";
	thing.UIHints.Description = "";
	thing.UIHints.IconURI = "/Content/img/icons/Generic/thing-genericbutton.png";

}

void send_button_event(bool isPushed)
{
	send_porteventmsg(ctx.ButtonThingPortKey, (isPushed ? "1" : "0"));
}


void publisher(String msg, String topic)
{
	Serial.printf("publishing to %s: %s\n", topic.c_str(), msg.c_str());
	if (client.publish(topic.c_str(), msg.c_str()))
	{
		Serial.println("Publish ok");
	}
	else {
		Serial.println("Publish failed");
	}
}


bool handle_portevent(String portKey, String state)
{
	Serial.println("Got msg for port:" + portKey);
	if (portKey == ctx.LedThingPortKey)
	{
		Serial.println("Got msg for LED. Lets switch the led:");
		if (state == "1")
			led_state = LOW; //to turn the led on we should set to low
		else if (state == "0")
			led_state = HIGH; //to turn the led off we should set to high
		else
			Serial.println("unexpected port state");

		digitalWrite(BUILTIN_LED_PIN, led_state);
	}
	else
	{
		Serial.println("can only handle the led thing port for now");
	}
	return true;
}
