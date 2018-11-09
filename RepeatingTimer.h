// Timer.h

#ifndef _TIMER_h
#define _TIMER_h

#include "arduino.h"
#include "Element.h"
#include <functional>

class RepeatingTimer : public Element
{
public:
	RepeatingTimer(int timeout, int maxRandomVariation = 0);
	std::function<void()> OnTick;
	virtual void Enable();

private:
	int _timeout;
	int _maxRandVar;
	int _randVar;
	unsigned long _lastTick;
protected:
	 virtual void Loop();

};


#endif

