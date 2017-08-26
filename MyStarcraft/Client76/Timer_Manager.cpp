#include "StdAfx.h"
#include "Timer_Manager.h"

#include "Timer.h"

IMPLEMENT_SINGLETON(CTimer_Manager)

CTimer_Manager::CTimer_Manager(void)
{
}

CTimer_Manager::~CTimer_Manager(void)
{
}

float CTimer_Manager::Get_TimeDelta(const TCHAR* pTimerTag)
{
	CTimer*			pTimer = NULL;

	pTimer = Find_Timer(pTimerTag);

	if(NULL == pTimer)
		return 0.0f;

	return pTimer->Get_TimeDelta();
}	

HRESULT CTimer_Manager::Add_Timer(const TCHAR* pTimerTag, CTimer* pTimer)
{
	if(NULL != Find_Timer(pTimerTag))
		return E_FAIL;

	m_mapTimer.insert(MAPTIMER::value_type(pTimerTag, pTimer));
	
	return NOERROR;
}

void CTimer_Manager::Compute_Timer(const TCHAR* pTimerTag)
{
	CTimer*			pTimer = NULL;

	pTimer = Find_Timer(pTimerTag);

	if(NULL == pTimer)
		return;

	pTimer->Compute_Timer();

}

CTimer* CTimer_Manager::Find_Timer(const TCHAR* pTimerTag)
{
	MAPTIMER::iterator iter = find_if(m_mapTimer.begin(), m_mapTimer.end(), STRING_COMPARE(pTimerTag));

	if(iter == m_mapTimer.end())
		return NULL;

	return iter->second;
}

void CTimer_Manager::Release(void)
{
	for ( auto iter = m_mapTimer.begin(); iter != m_mapTimer.end(); ++iter )
	{
		safe_release( iter->second );
	}

	delete this;
}
