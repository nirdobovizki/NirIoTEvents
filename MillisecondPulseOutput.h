#pragma once

#include "arduino.h"
#include "Element.h"
#include <functional>

class MillisecondPulseOutput : public Element
{
public:
	MillisecondPulseOutput(uint8_t pin, bool highIsOn);
	void MultiPulse(unsigned int pulseLength, unsigned int pulseDelay, unsigned int pulseCount);
	std::function<void()> OnPulseComplete;

protected:
	virtual void Loop();

private:
	uint8_t _pin;
	int _state;
	int _onValue;
	int _offValue;
	unsigned int _pulseLength;
	unsigned int _pulseDelay;
	unsigned int _pulseCount;
	unsigned long _lastToggle;
};

