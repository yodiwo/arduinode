
Included Libraries:

PubSubClient
Files PubSubClient.cpp and PubSubClient.h belong to the library PubSubClient, written by Nick O'Leary and distributed under the license written in the file PUBSUBCLIENT_LICENSE.txt

Additional library dependencies:
Users need to install the library ArduinoJson from the standard arduino repository (from Manage Libraries in the Arduino IDE)

Board setup:
A non standard Board Manager URL is needed: http://arduino.esp8266.com/stable/package_esp8266com_index.json
Board options:
	Tools-->Board-->BoardInfoESP8266 Community
	Tools-->Board-->Generic 8266 module
	Tools-->port : set the correct port from the device manager
	Tools->reset method:nodmcu

Configuration:
configuration at this time is written in the file config.h
WIFI_SSID
WIFI_PASSWORD
refer to the WiFi setup


USERKEY
NODEKEY
NODE_SECRET

are obtained from the Cyan environment, using the Create Node wizard (USERKEY is the NODEKEY part up to and not including the dash)
