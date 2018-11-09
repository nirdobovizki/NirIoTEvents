// GrowingArray.h

#ifndef _GROWINGARRAY_h
#define _GROWINGARRAY_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

template<typename T>
class GrowingArray
{
private:
	T* m_buffer;
	int m_count;
	int m_size;


public:
	int GetCount() { return m_count; }
	void Add(T& value)
	{
		if (m_count == m_size)
		{
			int newSize = m_size + 5;
			T* newData = (T*)malloc(sizeof(T)*newSize);
			memcpy(newData, m_buffer, sizeof(T)*m_size);
			free(m_buffer);
			m_buffer = newData;
			m_size = newSize;
		}
		m_buffer[m_count++] = value;
	}
	T& Get(int i) { return m_buffer[i]; }
};


#endif

