#include "SPIFFSWifiCredStorage.h"
#include "FS.h"

#define MAX_BUFFER 512

SPIFFSWifiCredStorage::SPIFFSWifiCredStorage()
{
}


SPIFFSWifiCredStorage::~SPIFFSWifiCredStorage()
{
}

bool SPIFFSWifiCredStorage::Read(String& ssid, String& password)
{
	bool success = false;
	SPIFFS.begin();
	auto file = SPIFFS.open("WifiCred", "r");
	if (!!file)
	{
		uint8_t version = 0;
		if (file.read(&version, 1) == 1)
		{
			switch (version)
			{
			case 1:
			{
				uint8_t ssidLen, passwordLen;
				if (file.read(&ssidLen, 1) == 1 && file.read(&passwordLen, 1) == 1)
				{
					char buffer[MAX_BUFFER];
					if (file.read((uint8_t*)buffer, ssidLen + 1) == ssidLen + 1)
					{
						if (buffer[ssidLen] == 0)
						{
							ssid = buffer;
							if (file.read((uint8_t*)buffer, passwordLen + 1) == passwordLen + 1)
							{
								if (buffer[passwordLen] == 0)
								{
									password = buffer;
									success = true;
								}
							}
						}
					}

				}
				break;
			}
			}
		}
		file.close();
	}
	SPIFFS.end();
	return success;
}

bool SPIFFSWifiCredStorage::Write(const String& ssid, const String& password)
{
	SPIFFS.begin();
	auto file = SPIFFS.open("WifiCred", "w");
	bool success = false;
	if (!!file)
	{
		uint8_t buffer[MAX_BUFFER] = { 0 };
		buffer[0] = 1;
		buffer[1] = ssid.length();
		buffer[2] = password.length();
		memcpy(buffer + 3, ssid.c_str(), ssid.length());
		buffer[ssid.length() + 3] = 0;
		memcpy(buffer + ssid.length() + 4, password.c_str(), password.length());
		buffer[ssid.length() + password.length() + 4] = 0;

		file.write(buffer, ssid.length() + password.length() + 5);
		file.close();
		success = true;
	}
	SPIFFS.end();
	return success;
}