// 
// 
// 

#include "Led.h"

enum LedStates
{
	State_FastBlinkOn=1,
	State_FastBlinkOff=2,
	State_SlowBlinkOn=3,
	State_SlowBlinkOff=4,
	State_On=5,
	State_Off=6
};

Led::Led(uint8_t pin, bool highIsOn) :
	_state(State_Off),
	_pin(pin),
	_onValue(highIsOn ? 1 : 0),
	_offValue(highIsOn ? 0 : 1),
	SlowBlinkDuration(1000),
	FastBlinkDuration(250)
{
	pinMode(pin, OUTPUT);
	digitalWrite(pin, _offValue);
}

void Led::On()
{
	_state = State_On;
	digitalWrite(_pin, _onValue);
}
void Led::Off()
{
	_state = State_Off;
	digitalWrite(_pin, _offValue);
}
void Led::SlowBlink()
{
	_state = State_SlowBlinkOn;
	_lastToggle = millis();
	digitalWrite(_pin, _onValue);
}
void Led::FastBlink()
{
	_state = State_FastBlinkOn;
	_lastToggle = millis();
	digitalWrite(_pin, _onValue);
}
void Led::Toggle()
{
	if (IsOn()) Off();
	else On();
}
bool Led::IsOn()
{
	return _state == State_On || _state == State_FastBlinkOn || _state == State_SlowBlinkOn;
}
void Led::Loop()
{
	if (_state == State_On||_state==State_Off) return;
	auto timeout = _state == State_FastBlinkOff||_state == State_FastBlinkOn ? FastBlinkDuration: SlowBlinkDuration;
	auto now = millis();
	if (now - _lastToggle > timeout)
	{
		_lastToggle = now;
		if (_state == State_FastBlinkOn||_state == State_SlowBlinkOn)
		{
			_state++;
			digitalWrite(_pin, _offValue);
		}
		else
		{
			_state--;
			digitalWrite(_pin, _onValue);
		}
	}
	
}