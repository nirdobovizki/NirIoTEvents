// 
// 
// 

#include "StringUtils.h"

String StringUtils::ProcessTemplate(const String& templateText, String keys[], String values[], int replaceLength)
{
	String result;
	int sectionStart = 0;
	int state = 0;
	for (int pos = 0;templateText[pos];++pos)
	{
		switch (state)
		{
		case 0:
			if (templateText[pos] == '{')
			{
				if(sectionStart<pos)
					result += templateText.substring(sectionStart, pos);
				sectionStart = pos + 1;
				state = 1;
			}
			break;
		case 1:
			if (templateText[pos] == '}')
			{
				auto key = templateText.substring(sectionStart, pos);
				Serial.println(String("Key:") + key);
				for (int i = 0; i < replaceLength; ++i)
				{
					if (key == keys[i])
					{
						Serial.println(String("Value:") + values[i]);
						result += values[i];
						break;
					}
				}
				sectionStart = pos + 1;
				state = 0;
			}
			break;
		}
	}
	if(sectionStart<templateText.length()-1)
		result += templateText.substring(sectionStart);
	return result;
}