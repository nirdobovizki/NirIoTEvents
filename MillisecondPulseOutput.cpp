#include "MillisecondPulseOutput.h"


enum PulseState
{
	State_Idle,
	State_TurnOn,
	State_InPulse,
	State_InPause
};

MillisecondPulseOutput::MillisecondPulseOutput(uint8_t pin, bool highIsOn) :
	_state(State_Idle),
	_pin(pin),
	_onValue(highIsOn ? 1 : 0),
	_offValue(highIsOn ? 0 : 1)
{
	pinMode(pin, OUTPUT);
	OnPulseComplete = []() {};
}

void MillisecondPulseOutput::MultiPulse(unsigned int pulseLength, unsigned int pulseDelay, unsigned int pulseCount)
{
	_pulseLength = pulseLength;
	_pulseDelay = pulseDelay;
	_pulseCount = pulseCount;
	_state = State_TurnOn;
}

void MillisecondPulseOutput::Loop()
{
	auto now = millis();
	switch (_state)
	{
	case State_TurnOn:
		digitalWrite(_pin, _onValue);
		_lastToggle = now;
		_state = State_InPulse;
		break;
	case State_InPulse:
		if (now - _lastToggle > _pulseLength)
		{
			digitalWrite(_pin, _offValue);
			_lastToggle = now;
			_state = --_pulseCount ? State_InPause : State_Idle;
			if (!_state) OnPulseComplete();
		}
		break;
	case State_InPause:
		if (now - _lastToggle > _pulseDelay)
		{
			_state = State_TurnOn;
		}
		break;
	default:
		break;
	}
}