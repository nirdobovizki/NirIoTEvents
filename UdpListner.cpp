#include "UdpListner.h"


UdpListner::UdpListner(uint16_t localUdpPort):
	_localUdpPort(localUdpPort),
	_active(false)
{
}


UdpListner::~UdpListner()
{
}

void UdpListner::Start()
{
	_udp.begin(_localUdpPort);
	_active = true;
}

void UdpListner::Loop()
{
	if (!_active) return;
	int packetSize = _udp.parsePacket();
	if (packetSize)
	{
		Serial.printf("Received %d bytes from %s, port %d\n", packetSize, _udp.remoteIP().toString().c_str(), _udp.remotePort());
		std::vector<char> incomingPacket;
		incomingPacket.resize(256);
		int len = _udp.read(&*incomingPacket.begin(), 255);
		if (len > 0)
		{
			incomingPacket[len] = 0;
		}
		//Serial.printf("UDP packet contents: %s\n", incomingPacket);
		if (OnMessage)
		{
			OnMessage(&incomingPacket[0], len);
		}
		Serial.println("Message processing complete");
		yield();
	}
}

void UdpListner::Broadcast(const char* buffer, size_t length)
{
	if (!_active) return;
	_udp.beginPacket(IPAddress(255, 255, 255, 255), _localUdpPort);
	_udp.write(buffer, length);
	_udp.endPacket();

}
