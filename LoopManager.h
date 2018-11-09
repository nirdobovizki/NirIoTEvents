// LoopManager.h

#ifndef _LOOPMANAGER_h
#define _LOOPMANAGER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "GrowingArray.h"
#include <functional>

class LoopManagerClass
{
 private:
	 GrowingArray<std::function<void()>> _callbacks;

 public:
	void Loop();
	void AddCallback(std::function<void()> toAdd);
	void Delay(int timeMilliseconds);
};

extern LoopManagerClass LoopManager;

#endif

