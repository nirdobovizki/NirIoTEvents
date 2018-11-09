// 
// 
// 

#include "WiFiConnection.h"
#include "ESP8266WiFi.h"

#if defined(DEBUG_WIFICONNECTION) || defined(DEBUG_ALL)
#if !defined(DEBUG_WRITER)
#define DEBUG_WRITER Serial.println
#endif
#define DebugOut(x) DEBUG_WRITER(x)
#else
#define DebugOut(x)
#endif

#define WAIT_FOR_RECONNECT_TIMEOUT 1000

enum InternetConnectionStates
{
	State_NotConnected,
	State_StartConnect,
	State_Connecting,
	State_Connected,
	State_WaitToReconnect

};

void* WiFiConnection::WiFiOwner;

WiFiConnection::WiFiConnection() : _state(State_NotConnected), _connectStart(0)
{
	OnConnected = OnDisconnected = OnConnectFailure = OnConnectStart = []() {};
}

void WiFiConnection::Loop()
{
	if (WiFiOwner != this) return;

	auto now = millis();
	switch (_state)
	{
	case State_NotConnected:
		break;
	case State_StartConnect:
		DebugOut("WiFiConection:\nAttempting to connect");
		_connectStart = now;
		OnConnectStart();
		WiFi.begin();
		_state = State_Connecting;
		break;
	case State_Connecting:
	case State_Connected:
		if (_state == State_Connecting && now - _connectStart > 10000)
		{
			DebugOut(String("WiFiConection:\nConnecting is taking too long, aborting, will retry later\nWiFi status: ") + String(WiFi.status()));
			WiFi.disconnect();
			OnConnectFailure();
			_state = State_WaitToReconnect;
			break;
		}
		switch (WiFi.status())
		{
		case WL_CONNECTED:
			if (_state == State_Connecting)
			{
				DebugOut(String("WiFiConection:\nConnected\nIP Address: ") + WiFi.localIP().toString() +
					"\nSubnet masks: " + WiFi.subnetMask().toString() + "\nGateway: " + WiFi.gatewayIP().toString() +
					"\nDNS servers: " + WiFi.dnsIP().toString() + ", " + WiFi.dnsIP(1).toString() +
					"\nHost name: " + WiFi.hostname());
				OnConnected();
				_state = State_Connected;
			}
			break;
		case WL_CONNECT_FAILED:
		case WL_CONNECTION_LOST:
			WiFi.disconnect();
			if (_state == State_Connected)
			{
				DebugOut(String("WiFiConection:\nDisconnected, will retry later\nWiFi status: ") + String(WiFi.status()));
				OnDisconnected();
			}
			if (_state == State_Connecting)
			{
				DebugOut(String("WiFiConection:\Connection failed, will retry later\nWiFi status: ") + String(WiFi.status()));
				OnConnectFailure();
			}
			_state = State_WaitToReconnect;
			_disconnectTime = now;
			break;
		}
		break;
	case State_WaitToReconnect:
		if (millis() - _disconnectTime > WAIT_FOR_RECONNECT_TIMEOUT) _state = State_StartConnect;
		break;
	default:
		break;
	}
}

void WiFiConnection::ConnectWiFi()
{
	WiFiOwner = this;
	if (_state == State_NotConnected || _state == State_WaitToReconnect)
	{
		DebugOut("WiFiConection:\nConnectWiFi called, starting connection");
		_state = State_StartConnect;
		WiFi.mode(WIFI_STA);
	}
	else
	{
		DebugOut("WiFiConection:\nAlready connected");
	}
}

bool WiFiConnection::IsConnected()
{
	return WiFi.status() == WL_CONNECTED;
}