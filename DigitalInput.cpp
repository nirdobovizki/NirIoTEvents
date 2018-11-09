// 
// 
// 

#include "DigitalInput.h"

DigitalInput::DigitalInput(uint8_t pin):_pin(pin),_prevState(0),_changeTime(0)
{
	OnHigh = OnLow = []() {};
}

void DigitalInput::Loop()
{
	auto state = digitalRead(_pin);
	auto now = millis();
	auto delta = now - _changeTime;
	if (state != _prevState && delta > 300)
	{
		_prevState = state;
		_changeTime = now;
		(state ? OnHigh : OnLow)();
	}
}