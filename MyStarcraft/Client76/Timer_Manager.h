#pragma once

// For.타이머들을 여러개 보관하기위해 만들어진 클래스
#include "Include.h"

class CTimer;
class CTimer_Manager
{
	DECLARE_SINGLETON(CTimer_Manager)
private:
	explicit CTimer_Manager(void);
	virtual ~CTimer_Manager(void);
public:
	float Get_TimeDelta(const TCHAR* pTimerTag);

public: //추가하겠다.
	HRESULT Add_Timer(const TCHAR* pTimerTag, CTimer* pTimer);
	void Compute_Timer(const TCHAR* pTimerTag);
private:
	map<const TCHAR*, CTimer*>			m_mapTimer;
	typedef map<const TCHAR*, CTimer*>	MAPTIMER;
private:
	CTimer* Find_Timer(const TCHAR* pTimerTag);
public:
	void Release(void);	
};
