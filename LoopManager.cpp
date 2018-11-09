// 
// 
// 

#include "LoopManager.h"

void LoopManagerClass::Loop()
{
	for (int i = 0; i < _callbacks.GetCount(); ++i)_callbacks.Get(i)();
}

void LoopManagerClass::AddCallback(std::function<void()> toAdd)
{
	_callbacks.Add(toAdd);
}

void LoopManagerClass::Delay(int timeMilliseconds)
{
	auto start = millis();
	while (millis()-start<timeMilliseconds)
	{
		yield();
		Loop();
	}
}


LoopManagerClass LoopManager;

