#pragma once

#include "arduino.h"
#include "Element.h"
#include <functional>


class OneShotTimer : public Element
{
public:
	OneShotTimer(unsigned long timeout, bool startFromBoot);
	void Stop();
	void Reset();
	std::function<void()> OnTick;
	unsigned long Timeout;

private:
	unsigned long _startTime;
	bool _active;

protected:
	virtual void Loop();
};

