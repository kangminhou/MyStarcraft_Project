#include "StdAfx.h"
#include "Timer.h"

CTimer::CTimer(void)
: m_fTimeDelta(0.f)
{
}

CTimer::~CTimer(void)
{
}

HRESULT CTimer::Initialize(void)
{
	// GetTickCount()
	QueryPerformanceCounter(&m_FrameTime);		//���� Cpu�� ī������ �ð��� �޾ƿ´�.
	QueryPerformanceCounter(&m_FixTime);		//������ �ʱ�ȭ�� �Ѵٰ� ��������!
	QueryPerformanceCounter(&m_LastTime);	

	QueryPerformanceFrequency(&m_CpuTick);		//CPU�� 1�ʿ� ī������ �� �ִ� Tick�� ������ �Լ�.

	//1�ʴ� �������� �Ǵ��ϱ� ���ؼ� ����Ѵ�.
	return NOERROR;
}


void CTimer::Compute_Timer(void)
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

	m_fTimeDelta = float( m_FrameTime.QuadPart - m_FixTime.QuadPart ) / m_CpuTick.QuadPart;

	m_FixTime	  = m_FrameTime;
}

CTimer* CTimer::Create(void)
{
	CTimer*		pInstance = new CTimer();

	if(FAILED(pInstance->Initialize()))
	{
		MessageBox(0, L"CTimer Created Failed", L"System Error", MB_OK);
		safe_release(pInstance);
	}

	return pInstance;
}

void CTimer::Release(void)
{	
	delete this;
}
