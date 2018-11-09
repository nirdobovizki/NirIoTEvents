// Button.h

#ifndef _BUTTON_h
#define _BUTTON_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "DigitalInput.h"

class Button : public Element
{
private:
	DigitalInput _input;
	unsigned long _pressTime;
	int _state;
 protected:

	 virtual void Loop();

 public:
	 Button(uint8_t pin, bool pressedIsHigh, bool pullup);

	 std::function<void()> OnPress;
	 std::function<void()> OnLongPress;
	 std::function<void()> OnClick;
	 std::function<void()> OnRelease;

	 unsigned int LongPressTime;
};

#endif

