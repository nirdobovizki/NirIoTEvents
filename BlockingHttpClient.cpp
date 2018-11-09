// 
// 
// 

#include "BlockingHttpClient.h"

enum HttpState
{
	State_Idle=0,
	State_ClosePrevAndConnect=1,
	State_Connect=2,
	State_SendReqHeader=3,
	State_SkipResponseBeforeStatusCode=4,
	State_StatusCodeDigit1 = 5,
	State_StatusCodeDigit2 = 6,
	State_StatusCodeDigit3 = 7,
	State_SkipHeaders=8,
	State_SkipHeadersNewLine=9,
	State_SkipHeadersFinalChar=10,
	State_FinishedHeaders=11,
	State_Data=12,
	State_Error
};

BlockingHttpClient::BlockingHttpClient() :
	_state(0),
	OnError([]() {}),
	OnStatusCode([](int s) {return true; }),
	OnData([](char* b, int s) {return true; }),
	OnDisconnected([]() {})
{
	_statusCode[3] = 0;
}

bool BlockingHttpClient::Get(String host, String url)
{
	if (_state != State_Idle) return false;

	Serial.println("Starting HTTP request");
	Serial.println(host);
	Serial.println(url);

	_host = host;
	_url = url;
	_state = State_Connect;
	return true;
}

void BlockingHttpClient::Loop()
{
	switch (_state)
	{
	case State_Idle:
		break;
	case State_ClosePrevAndConnect:
		_client.stop();
		_state = State_Connect;
		break;
	case State_Connect:
		if (_client.connect(_host.c_str(), 80))
		{
			_state = State_SendReqHeader;
		}
		else
		{
			_state = State_Error;
		}
		break;
	case State_SendReqHeader:
		_client.print(String("GET http://") + _host + "/" + _url + " HTTP/1.1\r\n" +
			"Host: " + _host + "\r\n" +
			"User-Agent: NirIoT Device\r\n" +
			"Connection: close\r\n\r\n");
		_state = State_SkipResponseBeforeStatusCode;
		break;
	case State_SkipResponseBeforeStatusCode:
	{
		if (!_client.connected()) _state = State_Error;
		if (!_client.available()) break;
		auto next = _client.read();
		if (next == ' ')
		{
			_state = State_StatusCodeDigit1;
			break;
		}
		break;

	}
	case State_StatusCodeDigit1:
	case State_StatusCodeDigit2:
	case State_StatusCodeDigit3:
	{
		if (!_client.connected()) _state = State_Error;
		if (!_client.available()) break;
		auto next = _client.read();
		if (next < 0)
		{
			_state = State_Error;
			break;
		}
		_statusCode[_state - State_StatusCodeDigit1] = (char)next;
		_state++;
	}
		break;
	case State_SkipHeaders:
		while (_client.available())
		{
			auto next = _client.read();
			if (next < 0)
			{
				_state = State_Error;
				break;
			}
			if (next == '\n')
			{
				_state = State_SkipHeadersNewLine;
				break;
			}
		}
		if (!_client.connected())
		{
			_state = State_Error;
		}
		break;
	case State_SkipHeadersNewLine:
	{
		if (!_client.connected()) _state = State_Error;
		if (!_client.available())break;
		auto next = _client.read();
		if (next < 0)
		{
			_state = State_Error;
			break;
		}
		if (next == '\r')
		{
			_state = State_SkipHeadersFinalChar;
		}
		else
		{
			_state = State_SkipHeaders;
		}
		break;
	}
	case State_SkipHeadersFinalChar:
	{
		if (!_client.connected()) _state = State_Error;
		if (!_client.available())break;
		auto next = _client.read();
		if (next != '\n')
		{
			_state = State_Error;
			break;
		}
		_state = State_FinishedHeaders;
		break;
	}
	case State_FinishedHeaders: 
		if (!OnStatusCode(atoi(_statusCode)))
		{
			_client.stop();
			_state = State_Idle;
			break;
		}
		_state = State_Data;
		break;
	case State_Data:
	{
		if (!_client.connected())
		{
			OnDisconnected();
			_client.stop();
			_state = State_Idle;
			break;
		}
		
		auto size = _client.available();
		if (size)
		{
			auto buffer = (char*)malloc(size);
			_client.read((uint8_t*)buffer, size);
			if (!OnData(buffer, size))
			{
				_client.stop();
				_state = State_Idle;
			}
			free(buffer);
		}
		break;
	}

	case State_Error:
		OnError();
		_client.stop();
		_state = State_Idle;
		break;


	default:
		break;
	}
}
