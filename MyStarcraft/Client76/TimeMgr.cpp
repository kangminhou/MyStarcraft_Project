#include "StdAfx.h"
#include "TimeMgr.h"

IMPLEMENT_SINGLETON(CTimeMgr);

float CTimeMgr::GetTime(void)
{
	return m_fTime;
}

float CTimeMgr::GetGlobalTime(void)
{
	return m_fGlobalTime;
}

void CTimeMgr::InitTime(void)
{
	
	//QueryPerformanceCounter() : 고해상도의 타이머를 통해서 현재값을 얻을수 있는 함수이다.
	//							  1 /10000			-> Cpu가 만들어낸 함수이다.
		
	//GetTickCount()  : 1 / 1000	 -> Os

	QueryPerformanceCounter(&m_FrameTime);		//최초 Cpu의 시간을 받아온다.
	QueryPerformanceCounter(&m_FixTime);		//지금은 초기화를 한다고 생각하자!
	QueryPerformanceCounter(&m_LastTime);	

	QueryPerformanceFrequency(&m_CpuTick);		//CPU의 Tick을 얻어오는 함수.
												//1초당 떨림수를 판단하기 위해서 사용한다.
}

void CTimeMgr::SetTime(void)
{
	//매 프레임, 즉 호출이 될때마다 새로운 시간을 구한다.
	QueryPerformanceCounter(&m_FrameTime);


	//새로 구한시간과 처음 Init되었을때의 시간 사이를 구한다.
	if( (m_FrameTime.QuadPart - m_LastTime.QuadPart) > m_CpuTick.QuadPart)
	{
		//프로세스 부하가 걸렸을때 시간이 뒤로 밀리는것을 방지하기 위한 분기문.
	
		//CPU Tick을 다시 갱신하다.
		QueryPerformanceFrequency(&m_CpuTick);

		//1초 뒤 값을 새롭게 갱신한다.
		m_LastTime.QuadPart = m_FrameTime.QuadPart;
	}

	//메인게임 Update() 호출되고 다음 호출까지의 인터벌값을 진동수로 나누게 된다면
	//우리가 프레임당 움직일수 있는 시간의 값이 나온다.

	m_fTime = float( m_FrameTime.QuadPart - m_FixTime.QuadPart ) / m_CpuTick.QuadPart;

	m_fGlobalTime += m_fTime;

	m_FixTime	  = m_FrameTime;
}

void CTimeMgr::Release(void)
{

}

CTimeMgr::CTimeMgr(void)
{
	m_fTime		  = 0.f;
	m_fGlobalTime = 0.f;
}

CTimeMgr::~CTimeMgr(void)
{
	Release();
}
