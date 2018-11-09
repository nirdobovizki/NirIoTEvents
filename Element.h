// Element.h

#ifndef _ELEMENT_h
#define _ELEMENT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class Element
{
 protected:
	 bool _enabled;
	 virtual void Loop();

 public:
	 Element();

	virtual void Disable();
	virtual void Enable();
	
};


#endif

