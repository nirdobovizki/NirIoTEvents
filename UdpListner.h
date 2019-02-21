#pragma once

#include "Element.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <vector>

class UdpListner : Element
{


public:
	UdpListner(uint16_t localUdpPort);
	~UdpListner();

	void Start();
	void Broadcast(const char* buffer, size_t length);

	std::function<void(const char*, size_t length)> OnMessage;

protected:

	virtual void Loop();

private:
	WiFiUDP _udp;
	uint16_t _localUdpPort;
	bool _active;
};

