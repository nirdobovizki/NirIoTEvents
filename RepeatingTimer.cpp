// 
// 
// 

#include "RepeatingTimer.h"

RepeatingTimer::RepeatingTimer(int timeout, int maxRandomVariation) : 
	_timeout(timeout), _maxRandVar(maxRandomVariation), _lastTick(millis()), _randVar(0)
{
	OnTick = []() {};
}

void RepeatingTimer::Loop()
{
	auto now = millis();
	if (now - _lastTick > _timeout + _randVar)
	{
		if (_maxRandVar != 0)
		{
			_randVar = (rand() % (_maxRandVar * 2)) - _maxRandVar;
		}
		_lastTick = now;
		OnTick();
	}
}

void RepeatingTimer::Enable()
{
	_lastTick = millis();
	Element::Enable();
}

