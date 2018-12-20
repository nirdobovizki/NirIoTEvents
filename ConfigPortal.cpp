// 
// 
// 

#include "ConfigPortal.h"
#include "ConfigPortalPages.h"
#include "StringUtils.h"
#include "WiFiConnection.h"
#include "IWiFiCredStorage.h"
#include <algorithm>

ConfigPortal::ConfigPortal(IWiFiCredStorage* credStorage) :
	_active(false),
	_dnsServer(NULL),
	_webServer(NULL),
	Ssid("NirIotDevice"),Password("1234567890"),
	Title("Network Configuration")
{
	_credStorage = credStorage;
	OnNetworkActivity = []() {};
}

void ConfigPortal::Start()
{
	if (_active) return;
	_active = true;
	WiFiConnection::WiFiOwner = this;

	WiFi.disconnect();
	WiFi.mode(WIFI_AP_STA);
	WiFi.softAP(Ssid.c_str(), Password.c_str());

	delay(10);


	_dnsServer = new DNSServer();
	_dnsServer->setErrorReplyCode(DNSReplyCode::NoError);
	_dnsServer->start(53, "*", WiFi.softAPIP());

	_webServer = new ESP8266WebServer(80);
	_webServer->on("/", std::bind(&ConfigPortal::HandleRoot, this));
	_webServer->on("/save", std::bind(&ConfigPortal::HandleSave, this));
	_webServer->on("/style", std::bind(&ConfigPortal::HandleStyle, this));
	_webServer->onNotFound(std::bind(&ConfigPortal::HandleNotFound, this));
	_webServer->begin();

	Serial.println(String("Configuration Portal Started\nNetwork: ") + Ssid +
		"\nPassword: " + Password + "\nIP Address: " + WiFi.softAPIP());
}

void ConfigPortal::Stop()
{
	_dnsServer->stop();
	_webServer->stop();
	WiFi.mode(WIFI_STA);
	delete _dnsServer;
	delete _webServer;
	_dnsServer = NULL;
	_webServer = NULL;
	_active = false;
}

void ConfigPortal::Loop()
{
	if (_active)
	{
		_dnsServer->processNextRequest();
		_webServer->handleClient();
	}
}

void ConfigPortal::HandleRoot()
{
	if (DoRedirect())return;
	auto networkCount = WiFi.scanNetworks();
	String networkOptions;
	for (int i = 0; i < networkCount; ++i)
	{
		static String networkFregmntKeys[] = { String("ssid"),String("quality"),String("selected") };
		String networkFregmntValues[] = { WiFi.SSID(i), String(std::max(0, std::min(100, 2 * (WiFi.RSSI(i) + 100)))), String("") };
		networkOptions += StringUtils::ProcessTemplate(String(CP_NetworkFragment),
			networkFregmntKeys,
			networkFregmntValues, 3);
	}

	static String bodyKeys[] = { "networks", "title" };
	String bodyValues[] = { networkOptions, Title };
	String body = StringUtils::ProcessTemplate(String(CP_RootBody),
		bodyKeys ,
		bodyValues , 2);
	_webServer->send(200, "text/html", BuildPage(body));
}

void ConfigPortal::HandleSave()
{
	if (DoRedirect())return;
	auto ssid = _webServer->arg("ssid");
	auto password = _webServer->arg("password");
	WiFi.begin(ssid.c_str(), password.c_str());
	auto connectStart = millis();
	while (WiFi.status() != WL_CONNECTED && millis() - connectStart < 10000) delay(10);
	if (WiFi.status() == WL_CONNECTED)
	{
		_credStorage->Write(ssid, password);
		Serial.println("Connection successful");
		_webServer->send(200, "text/html", BuildPage(CP_SaveSuccessBody));
		delay(10000);
		ESP.reset();
	}
	else
	{
		Serial.println("Connection failed");
		_webServer->send(200, "text/html", BuildPage(CP_SaveFailedBody));
	}
}

void ConfigPortal::HandleNotFound()
{
	if (DoRedirect())return;
	_webServer->send(404, "text/html", BuildPage(CP_NotFoundBody));
}

void ConfigPortal::HandleStyle()
{
	if (DoRedirect())return;
	_webServer->send(200, "text/css", CP_Stylesheet);
}

bool ConfigPortal::DoRedirect()
{
	OnNetworkActivity();
	Serial.println(String("Web request:") + _webServer->method() + " " + _webServer->uri());
	Serial.println(String("Host:") + _webServer->hostHeader());
	Serial.println(String(_webServer->headers()) + " headers, " + String(_webServer->args()) + " arguments");
	Serial.println("Headers:");
	for (int i = 0; i < _webServer->headers(); ++i)
	{
		Serial.println(String("   \"") + _webServer->headerName(i) + "\"=\"" + _webServer->header(i) + "\"");
	}
	Serial.println("Arguments:");
	for (int i = 0; i < _webServer->args(); ++i)
	{
		Serial.println(String("   \"") + _webServer->argName(i) + "\"=\"" + _webServer->arg(i) + "\"");
	}

	auto requestHost = _webServer->hostHeader();
	if (requestHost.length() && requestHost != "deviceconfig.nirapps.com")
	{
		_webServer->sendHeader("Location", String("http://deviceconfig.nirapps.com/"), true);
		_webServer->send(302, "text/plain", "");
		Serial.println("Redirecting");
		Serial.println();
		return true;
	}

	Serial.println();
	return false;
}

String ConfigPortal::BuildPage(const String& body)
{
	static String keys[] = { "title" };
	String values[] = { Title };
	return StringUtils::ProcessTemplate(String(CP_PageHeader), keys, values, 1) + body + CP_PageFooter;
}