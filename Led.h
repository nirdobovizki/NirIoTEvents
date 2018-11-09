// Led.h

#ifndef _LED_h
#define _LED_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Element.h"

class Led : Element
{
private:
	uint8_t _pin;
	int _state;
	int _onValue;
	int _offValue;
	unsigned long _lastToggle;

 protected:

	 virtual void Loop();


 public:
	 Led(uint8_t pin, bool highIsOn);

	 void On();
	 void Off();
	 void SlowBlink();
	 void FastBlink();
	 void Toggle();
	 bool IsOn();

	 unsigned int SlowBlinkDuration;
	 unsigned int FastBlinkDuration;
};


#endif

