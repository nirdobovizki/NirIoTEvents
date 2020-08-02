// 
// 
// 

#include "Button.h"

enum ButtonState
{
	State_Released,
	State_Prepressed,
	State_Pressed,
	State_LongPressReported,
	State_AfterRelease,
	State_AfterReleasePressed
};

Button::Button(uint8_t pin, bool pressedIsHigh, bool pullup) : 
	_input(pin), 
	_state(State_Released),
	LongPressTime(1000),
	IgnoreAfterRelease(0),
	IgnoreShortPresses(0)
{
	pinMode(pin, pullup ? INPUT_PULLUP : INPUT);
	OnPress = OnRelease = OnClick = OnLongPress = []() {};


	std::function<void()> onPress = [this]()
	{
		switch (_state)
		{
		case State_Released:
			_pressTime = millis();
			if (IgnoreShortPresses == 0)
			{
				_state = State_Pressed;
				OnPress();
			}
			else
			{
				_state = State_Prepressed;
			}
			break;
		case State_AfterRelease:
			_state = State_AfterReleasePressed;
			break;
		case State_Prepressed:
		case State_Pressed:
		case State_LongPressReported:
		case State_AfterReleasePressed:
			// shouldn't happen
			break;

		default:
			break;
		}
	};

	std::function<void()> onRelease = [this]()
	{
		switch (_state)
		{
		case State_Released:
		case State_AfterRelease:
			// shouldn't happen
			break;

		case State_Prepressed:
			_state = State_Released;
			break;
		
		case State_Pressed:
			OnRelease();
			OnClick();
			_state = State_AfterRelease;
			_pressTime = millis();
			break;

		case State_LongPressReported:
			OnRelease();
			_state = State_AfterRelease;
			_pressTime = millis();
			break;

		case State_AfterReleasePressed:
			_state = State_AfterRelease;
			break;
		default:
			break;
		}
	};

	_input.OnHigh = pressedIsHigh ? onPress : onRelease;
	_input.OnLow = pressedIsHigh ? onRelease : onPress;
}

void Button::Loop()
{
	_input.Update();
	if (_state == State_Pressed && millis() - _pressTime > LongPressTime)
	{
		_state = State_LongPressReported;
		OnLongPress();
	}
	if (_state == State_Prepressed && millis() - _pressTime > IgnoreShortPresses)
	{
		_state = State_Pressed;
		OnPress();
	}
	if (_state == State_AfterRelease && millis() - _pressTime > IgnoreAfterRelease)
	{
		_state = State_Released;
	}
	if (_state == State_AfterReleasePressed && millis() - _pressTime > IgnoreAfterRelease)
	{
		// don't know, another press?
	}
}