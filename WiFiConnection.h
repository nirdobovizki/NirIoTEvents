// InternetConnection.h

#ifndef _INTERNETCONNECTION_h
#define _INTERNETCONNECTION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Element.h"
#include <functional>

class WiFiConnection : Element
{
private:
	unsigned long _connectStart;
	int _state;
	unsigned long _disconnectTime;
 protected:

	 virtual void Loop();

 public:
	 WiFiConnection();

	 std::function<void()> OnConnected;
	 std::function<void()> OnDisconnected;
	 std::function<void()> OnConnectFailure;
	 std::function<void()> OnConnectStart;

	 void ConnectWiFi();
	 bool IsConnected();

	 static void* WiFiOwner;
	 
};


#endif

