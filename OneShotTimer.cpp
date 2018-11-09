#include "OneShotTimer.h"

OneShotTimer::OneShotTimer(unsigned long timeout, bool startFromBoot):
	Timeout(timeout),
	_startTime(0),
	_active(startFromBoot),
	OnTick([]() {})
{

}

void OneShotTimer::Stop()
{
	_active = false;
}

void OneShotTimer::Reset()
{
	_startTime = millis();
	_active = true;
}

void OneShotTimer::Loop()
{
	if (!_active) return;
	auto now = millis();
	if (now - _startTime > Timeout)
	{
		_active = false;
		OnTick();
	}

}
