#pragma once

#include "arduino.h"

class IWiFiCredStorage
{
public:
	virtual bool Read(String& ssid, String& password) = 0;
	virtual bool Write(const String& ssid, const String& password) = 0;
};



