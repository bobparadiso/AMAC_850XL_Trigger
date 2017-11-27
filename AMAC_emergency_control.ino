//running on NodeMCU 1.0
//using WeMo emulator code by Aruna Tennakoonfrom: https://github.com/kakopappa/arduino-esp8266-alexa-multiple-wemo-switch

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <functional>
#include "switch.h"
#include "UpnpBroadcastResponder.h"
#include "CallbackFunction.h"
#include "Servo.h"

int RFsignal_emergency[] = {
	300, 300,
	700, 700,
	300, 700,
	300, 700,
	300, 10900,
	400, 600,
	400, 300,
	700, 300,
	700, 300,
	700, 300,
	700, 300,
	700, 300,
	700, 600,
	300, 300,
	700, 600,
	300, 600,
	300, 600,
	300, 10900,
	400, 600,
	400, 300,
	700, 300,
	700, 300,
	700, 300,
	700, 300,
	700, 300,
	700, 600,
	300, 300,
	700, 600,
	300, 700,
	300, 600,
	300, 10900,
	400, 600,
	400, 300,
	700, 300,
	700, 300,
	700, 300,
	700, 300,
	700, 300,
	700, 600,
	300, 300,
	700, 600,
	300, 600,
	300, 700,
	300, 10900,
	400, 600,
	400, 300,
	700, 300,
	700, 300,
	700, 300,
	700, 300,
	700, 300,
	700, 600,
	300, 300,
	700, 600,
	300, 700,
	300, 600,
	300, 10900,
	400, 600,
	400, 300,
	700, 300,
	700, 300,
	700, 300,
	700, 300,
	700, 300,
	700, 600,
	300, 300,
	700, 600,
	300, 600,
	300, 700,
	300, 10900,
	400, 600,
	400, 300,
	700, 300,
	700, 300,
	700, 300,
	700, 300,
	700, 300,
	700, 600,
	300, 300,
	700, 700,
	300, 600,
	300, 600,
	300, 0};

#define RF_txPin D7

// prototypes
void setupWiFi();
void setupSwitches();

//on/off callbacks 
void switch1On();
void switch1Off();

// Change these before you flash
//#define FIRST_RUN
const char* ssid = "******";
const char* password = "**********";

UpnpBroadcastResponder upnpBroadcastResponder;
Switch *switch1 = NULL;

//
void setup()
{
	Serial.begin(9600);
	Serial.println("setup");
	//Serial.setDebugOutput(true);

	pinMode(RF_txPin, OUTPUT);

	setupWiFi();
	setupSwitches();
	Serial.println("ready");
	
	//test
	//switch1On();
}
 
void loop()
{
	if (WiFi.status() != WL_CONNECTED)
		return;
	
	upnpBroadcastResponder.serverLoop();
	switch1->serverLoop();
}

// connect to wifi – returns true if successful or false if not
void setupWiFi()
{
	boolean state = true;

#ifdef FIRST_RUN
	Serial.println("configuring WiFi chip");
	WiFi.mode(WIFI_STA);
	WiFi.setAutoReconnect(true);	  
	WiFi.begin(ssid, password);
	Serial.println("WiFi chip configured");
#endif
	
	Serial.println("");
	Serial.println("Connecting to WiFi");

	// Wait for connection
	Serial.print("Connecting ...");
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(100);
		Serial.print(".");
	}

	Serial.println("");
	Serial.print("Connected to ");
	Serial.println(ssid);
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());
}

//
void setupSwitches()
{
	upnpBroadcastResponder.beginUdpMulticast();

	// Define your switches here. Max 14
	// Format: Alexa invocation name, local port no, on callback, off callback
	switch1 = new Switch("emergency", 80, switch1On, switch1Off);

	Serial.println("Adding switches upnp broadcast responder");
	upnpBroadcastResponder.addDevice(*switch1);
}

//trigger AMAC either way
void switch1On()
{
	Serial.print("Switch 1 turn on ...");
	SendCode_Raw(RFsignal_emergency);
}

//trigger life alert either way
void switch1Off()
{
	Serial.print("Switch 1 turn off ...");
	SendCode_Raw(RFsignal_emergency);
}

// 
void SendCode_Raw(const int *code)
{
//   Serial.println("\nSendCode");

  const int *pulse = code;
  while (true)
  {
    int on = *(pulse++);
    int off = *(pulse++);

//    Serial.print("on:");
//    Serial.println(on);
//    Serial.print("off:");
//    Serial.println(off);

	digitalWrite(RF_txPin, HIGH);
	delayMicroseconds(on);
	digitalWrite(RF_txPin, LOW);
	delayMicroseconds(off);

    if (off == 0)
      return;
  }
}
