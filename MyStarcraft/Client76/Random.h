#pragma once

#ifndef __RANDOM_H__
#define __RANDOM_H__

#include "Include.h"
#include <ctime>

class CRandom
{
private:
	enum
	{
		DIFFER_VALUE = 100,
	};

protected:
	unsigned long	state[16];
	unsigned int	index;
	static CRandom* m_pInstance;

	explicit CRandom() {

		// 시드 값 자동 생성..
		index = 0;
		unsigned int curTime = static_cast<unsigned int>(time(NULL));

		for (int i = 0; i < 16; ++i) {

			state[i] = curTime;
			curTime += curTime + DIFFER_VALUE;

		}

	}

	explicit CRandom(unsigned int seed)
	{
		// 시드 값을 받아 초기화..
		index = 0;
		unsigned int s = seed;

		for (int i = 0; i < 16; ++i) {

			state[i] = s;
			s += s + DIFFER_VALUE;

		}

	}

public:									
	static CRandom* GetInstance()
	{
		if(NULL == m_pInstance)
			m_pInstance = new CRandom;
		return m_pInstance;
	}

	void DestroyInstance()
	{
		if(m_pInstance)					
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}

	unsigned int GetValue() {
		unsigned int a, b, c, d;
		a = state[index];
		c = state[(index + 13) & 15];
		b = a^c ^ (a << 16) ^ (c << 15);
		c = state[(index + 9) & 15];
		c ^= (c >> 11);
		a = state[index] = b^c;
		d = a ^ ((a << 5) & 0xDA442D20UL);
		index = (index + 15) & 15;
		a = state[index];
		state[index] = a^b^d ^ (a << 2) ^ (b << 18) ^ (c << 28);
		return state[index];

	}

	unsigned int GetValue(unsigned int minValue, unsigned int maxValue) {
		if ( maxValue == minValue )
			return minValue + (GetValue() % maxValue);

		return minValue + (GetValue() % (maxValue - minValue));
	}

	unsigned int GetValue(unsigned int maxValue) {
		return GetValue() % maxValue;
	}

	float GetFloatValue() {

		unsigned int randValue = GetValue();

		union { unsigned long ul; float f; } p;
		p.ul = (((randValue *= 16807) & 0x007fffff) - 1) | 0x3f800000;

		return p.f - 1.f;

	}
	float GetFloatValue(float minValue, float maxValue) {
		return minValue + GetFloatValue() * (maxValue - minValue);
	}

	float GetFloatValue(float maxValue) {
		return GetFloatValue() * maxValue;
	}
	
};

#endif