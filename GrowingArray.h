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
	GrowingArray() :m_buffer(NULL), m_count(0), m_size(0) {}
	int GetCount() { return m_count; }
	void Add(T& value)
	{
		if (m_count == m_size)
		{
			int newSize = m_size + 5;
			T* newData = new T[newSize];
			if (m_buffer != NULL)
			{
				for (size_t i = 0; i < m_count; ++i) newData[i] = m_buffer[i];
				delete[] m_buffer;
			}
			m_buffer = newData;
			m_size = newSize;
		}
		m_buffer[m_count++] = value;
	}
	T& Get(int i) { return m_buffer[i]; }
};


#endif

