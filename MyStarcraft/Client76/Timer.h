#pragma once

#include "Include.h"

// 클래스 헤더를 정의하는 방식

// 0. 상수(enum, const), macro
// 1. 생성자들
// 2. 소멸자
// -  Getter
// -  Setter
// 3. public 변수
// 4. public 함수
// 5. protected 변수
// 6. protected 함수
// 7. private 변수
// 8. private 함수
// 9. 이 클래스를 객체화하기위한 함수
// 10. 객체를 메모리해제하기위한 함수



class CTimer
{
private:
	explicit CTimer(void);
	virtual ~CTimer(void);
public:
	float Get_TimeDelta(void) const {
		return m_fTimeDelta;}
public:
	HRESULT Initialize(void);
private:
	LARGE_INTEGER 			m_FrameTime;
	LARGE_INTEGER 			m_FixTime;
	LARGE_INTEGER 			m_LastTime;
	LARGE_INTEGER 			m_CpuTick;
private:
	float					m_fTimeDelta;
public:
	void Compute_Timer(void); // 현재 타임델타를계산한다. // 매프레임마다 호출

public: // 생성과 관련된 함수.
	static CTimer* Create(void);
public:
	void Release(void);

};
