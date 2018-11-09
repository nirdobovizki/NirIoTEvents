// StringUtils.h

#ifndef _STRINGUTILS_h
#define _STRINGUTILS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class StringUtils
{
 
 public:
	static String ProcessTemplate(const String& templateText,String keys[], String values[], int replaceLength);
};


#endif

