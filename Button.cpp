// 
// 
// 

#include "Button.h"

enum ButtonState
{
	State_Released,
	State_Pressed,
	State_LongPressReported
};

Button::Button(uint8_t pin, bool pressedIsHigh, bool pullup) : 
	_input(pin), 
	_state(State_Released),
	LongPressTime(1000)
{
	pinMode(pin, pullup ? INPUT_PULLUP : INPUT);
	OnPress = OnRelease = OnClick = OnLongPress = []() {};


	std::function<void()> onPress = [this]()
	{
		_pressTime = millis();
		_state = State_Pressed;
		OnPress();
	};

	std::function<void()> onRelease = [this]()
	{
		if (_state)
		{
			OnRelease();
			if (_state != State_LongPressReported)
			{
				OnClick();
			}
			_state = State_Released;
		}
	};

	_input.OnHigh = pressedIsHigh ? onPress : onRelease;
	_input.OnLow = pressedIsHigh ? onRelease : onPress;
}

void Button::Loop()
{
	if (_state == State_Pressed && millis() - _pressTime > LongPressTime)
	{
		_state = State_LongPressReported;
		OnLongPress();
	}
}