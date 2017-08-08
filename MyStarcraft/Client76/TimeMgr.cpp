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
	
	//QueryPerformanceCounter() : ���ػ��� Ÿ�̸Ӹ� ���ؼ� ���簪�� ������ �ִ� �Լ��̴�.
	//							  1 /10000			-> Cpu�� ���� �Լ��̴�.
		
	//GetTickCount()  : 1 / 1000	 -> Os

	QueryPerformanceCounter(&m_FrameTime);		//���� Cpu�� �ð��� �޾ƿ´�.
	QueryPerformanceCounter(&m_FixTime);		//������ �ʱ�ȭ�� �Ѵٰ� ��������!
	QueryPerformanceCounter(&m_LastTime);	

	QueryPerformanceFrequency(&m_CpuTick);		//CPU�� Tick�� ������ �Լ�.
												//1�ʴ� �������� �Ǵ��ϱ� ���ؼ� ����Ѵ�.
}

void CTimeMgr::SetTime(void)
{
	//�� ������, �� ȣ���� �ɶ����� ���ο� �ð��� ���Ѵ�.
	QueryPerformanceCounter(&m_FrameTime);


	//���� ���ѽð��� ó�� Init�Ǿ������� �ð� ���̸� ���Ѵ�.
	if( (m_FrameTime.QuadPart - m_LastTime.QuadPart) > m_CpuTick.QuadPart)
	{
		//���μ��� ���ϰ� �ɷ����� �ð��� �ڷ� �и��°��� �����ϱ� ���� �б⹮.
	
		//CPU Tick�� �ٽ� �����ϴ�.
		QueryPerformanceFrequency(&m_CpuTick);

		//1�� �� ���� ���Ӱ� �����Ѵ�.
		m_LastTime.QuadPart = m_FrameTime.QuadPart;
	}

	//���ΰ��� Update() ȣ��ǰ� ���� ȣ������� ���͹����� �������� ������ �ȴٸ�
	//�츮�� �����Ӵ� �����ϼ� �ִ� �ð��� ���� ���´�.

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
