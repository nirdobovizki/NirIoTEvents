#pragma once

#include "IWiFiCredStorage.h"

class SPIFFSWifiCredStorage : public IWiFiCredStorage
{
public:
	SPIFFSWifiCredStorage();
	~SPIFFSWifiCredStorage();

	virtual bool Read(String& ssid, String& password);
	virtual bool Write(const String& ssid, const String& password);

};

