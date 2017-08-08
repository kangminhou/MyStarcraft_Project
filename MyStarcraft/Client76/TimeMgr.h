#pragma once

#include "Include.h"

class CTimeMgr
{
	DECLARE_SINGLETON(CTimeMgr);

	//DWORD dwTime;

private:
	LARGE_INTEGER m_FrameTime;
	LARGE_INTEGER m_FixTime;
	LARGE_INTEGER m_LastTime;
	LARGE_INTEGER m_CpuTick;
	float		  m_fTime;

	float		  m_fGlobalTime;

public:
	float GetTime(void);
	float GetGlobalTime(void);
	void  InitTime(void);
	void  SetTime(void);
	void  Release(void);

private:
	CTimeMgr(void);
public:
	~CTimeMgr(void);
};
