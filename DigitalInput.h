// DigitalInput.h

#ifndef _DIGITALINPUT_h
#define _DIGITALINPUT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <functional>
#include "Element.h"

class DigitalInput : public Element
{
private:
	uint8_t _pin;
	int _prevState;
	unsigned long _changeTime;
protected:

	virtual void Loop();

 public:
	 DigitalInput(uint8_t pin);

	 std::function<void()> OnHigh;
	 std::function<void()> OnLow;

	 void Update();

};


#endif

