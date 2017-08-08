#include "stdafx.h"
#include "Animation.h"

#include "TimeMgr.h"


CAnimation::CAnimation()
	: m_pCurAnimation(NULL)
{
}


CAnimation::~CAnimation()
{
	Release();
}

void CAnimation::SetAnimationData( const wstring & _wstrName, const FRAME & _tFrame )
{
	auto iterFindAnimation = m_mapAnimation.find( _wstrName );

	/* map �����̳ʿ� ���� ��쿡��.. */
	if ( iterFindAnimation != m_mapAnimation.end() )
	{
		iterFindAnimation->second->tFrame.fCount = _tFrame.fCount;
		iterFindAnimation->second->tFrame.fMax = _tFrame.fMax;
	}
}

const FRAME * CAnimation::GetCurAnimation()
{
	return &m_pCurAnimation->tFrame;
}

void CAnimation::Initialize()
{
	
}

void CAnimation::UpdateAnim()
{
	if ( this->m_pCurAnimation )
	{
		this->m_pCurAnimation->tFrame.fIndex += GET_TIME * this->m_pCurAnimation->tFrame.fCount;

		if ( this->m_pCurAnimation->tFrame.fIndex > this->m_pCurAnimation->tFrame.fMax )
			this->m_pCurAnimation->tFrame.fIndex = this->m_pCurAnimation->tFrame.fStart;
	}
}

void CAnimation::Release()
{
	for ( auto iter : m_mapAnimation )
	{
		safe_delete( iter.second );
	}
}

void CAnimation::AddAnimation( const wstring& _wstrName, const FRAME& _tFrame, const eAnim_Kind& eAnimKind )
{
	/* �̹� �ִ� �Ŷ�� ���ְԲ� ���� ó��.. */
	if ( m_mapAnimation.find( _wstrName ) != m_mapAnimation.end() )
		return;

	/* �ִϸ��̼� map �����̳ʿ� ���.. */
	//FRAME* pTempFrame = new FRAME( _tFrame );
	ANIM_DATA*	pAnimData = new ANIM_DATA;
	pAnimData->tFrame = _tFrame;
	pAnimData->eKind = eAnimKind;
	m_mapAnimation.insert( make_pair( _wstrName, pAnimData ) );
}

bool CAnimation::ChangeAnimation( const wstring & _wstrName )
{
	auto iterFindAnimation = m_mapAnimation.find( _wstrName );

	/* ã�� �ִϸ��̼��� ���� �ִϸ��̼��̶��.. */
	if ( iterFindAnimation == m_mapAnimation.end() )
		return false;

	if ( iterFindAnimation->second == m_pCurAnimation )
		return false;

	m_pCurAnimation = iterFindAnimation->second;
	m_pCurAnimation->tFrame.fIndex = 0.f;
	return true;
}
