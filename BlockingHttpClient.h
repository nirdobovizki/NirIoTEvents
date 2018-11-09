// Http.h

#ifndef _HTTP_h
#define _HTTP_h

#include "arduino.h"

#include "Element.h"
#include "ESP8266WiFi.h"
#include <functional>

class BlockingHttpClient : Element
{
private:
	int _state;
	char _statusCode[4];
	WiFiClient _client;
	String _host;
	String _url;

 protected:
	 virtual void Loop();

 public:
	 BlockingHttpClient();
	 bool Get(String host, String url);

	 std::function<void()> OnError;
	 std::function<bool(int)> OnStatusCode;
	 std::function<bool(char*, int)> OnData;
	 std::function<void()> OnDisconnected;

};


#endif

