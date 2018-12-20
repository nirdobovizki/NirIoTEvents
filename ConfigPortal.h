// CaptivePortal.h
#pragma once

#include "arduino.h"
#include "Element.h"
#include <functional>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>

class IWiFiCredStorage;


class ConfigPortal : Element
{
 protected:

	 virtual void Loop();

 public:
	 ConfigPortal(IWiFiCredStorage* credStorage);

	 void Start();
	 void Stop();

	 String Title;
	 String Ssid;
	 String Password;

	 std::function<void()> OnNetworkActivity;

private:
	bool _active;
	DNSServer* _dnsServer;
	ESP8266WebServer* _webServer;
	IWiFiCredStorage* _credStorage;

	bool DoRedirect();
	String BuildPage(const String& body);

	void HandleRoot();
	void HandleSave();
	void HandleStyle();
	void HandleNotFound();
};



