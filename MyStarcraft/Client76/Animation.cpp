#include "stdafx.h"
#include "Animation.h"

#include "TimeMgr.h"


CAnimation::CAnimation()
	: m_pCurAnimation(NULL)
	, m_bAnimEnd(false)
{
}


CAnimation::~CAnimation()
{
	Release();
}

void CAnimation::SetAnimationData( const wstring & _wstrName, const FRAME & _tFrame )
{
	auto iterFindAnimation = m_mapAnimation.find( _wstrName );

	/* map 컨테이너에 있을 경우에만.. */
	if ( iterFindAnimation != m_mapAnimation.end() )
	{
		iterFindAnimation->second->tFrame.fCount = _tFrame.fCount;
		iterFindAnimation->second->tFrame.fMax = _tFrame.fMax;
	}
}

void CAnimation::ChangeAnimationSpeed( const wstring & _wstrName, const float & _fSpeed )
{
	auto iterFindAnimation = m_mapAnimation.find( _wstrName );

	/* map 컨테이너에 있을 경우에만.. */
	if ( iterFindAnimation != m_mapAnimation.end() )
	{
		iterFindAnimation->second->tFrame.fCount = _fSpeed;
	}
}

void CAnimation::ResetFrame()
{
	if ( this->m_bPauseAnim )
		return;

	if ( this->m_pCurAnimation )
	{
		if ( !this->m_bCurFrame_Is_ReverseAnim )
			this->m_pCurAnimation->tFrame.fIndex = this->m_pCurAnimation->tFrame.fStart;
		else
			this->m_pCurAnimation->tFrame.fIndex = this->m_pCurAnimation->tFrame.fMax - 0.1f;
	}

	this->m_bAnimEnd = false;
}

const FRAME* CAnimation::GetCurAnimation()
{
	return &m_pCurAnimation->tFrame;
}

const bool CAnimation::GetIsAnimEnd() const
{
	return this->m_bAnimEnd;
}

void CAnimation::Initialize()
{
	this->m_bPauseAnim = false;
	this->m_bCurFrame_Is_ReverseAnim = false;
	this->m_bAnimEnd = false;
}

void CAnimation::UpdateAnim()
{
	if ( this->m_bPauseAnim )
		return;

	if ( this->m_pCurAnimation )
	{
		if ( !m_bCurFrame_Is_ReverseAnim )
			this->Update();
		else
			this->UpdateReverseAnim();
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
	/* 이미 있는 거라면 못넣게끔 예외 처리.. */
	if ( m_mapAnimation.find( _wstrName ) != m_mapAnimation.end() )
		return;

	/* 애니메이션 map 컨테이너에 담기.. */
	//FRAME* pTempFrame = new FRAME( _tFrame );
	ANIM_DATA*	pAnimData = new ANIM_DATA;
	pAnimData->tFrame = _tFrame;
	pAnimData->eKind = eAnimKind;
	m_mapAnimation.insert( make_pair( _wstrName, pAnimData ) );
}

bool CAnimation::ChangeAnimation( const wstring & _wstrName )
{
	auto iterFindAnimation = m_mapAnimation.find( _wstrName );

	/* 찾는 애니메이션이 없는 애니메이션이라면.. */
	if ( iterFindAnimation == m_mapAnimation.end() )
		return false;

	if ( iterFindAnimation->second == m_pCurAnimation )
		return false;

	m_pCurAnimation = iterFindAnimation->second;

	if ( m_pCurAnimation->eKind == Anim_Loop || m_pCurAnimation->eKind == Anim_ClampForever )
		this->m_bCurFrame_Is_ReverseAnim = false;
	else
		this->m_bCurFrame_Is_ReverseAnim = true;

	this->ResetFrame();
	return true;
}

void CAnimation::PauseAnim()
{
	this->m_bPauseAnim = true;
}

void CAnimation::StopPauseAnim()
{
	this->m_bPauseAnim = false;
}

void CAnimation::Update()
{
	this->m_pCurAnimation->tFrame.fIndex += GET_TIME * this->m_pCurAnimation->tFrame.fCount;

	if ( this->m_pCurAnimation->tFrame.fIndex >= this->m_pCurAnimation->tFrame.fMax )
	{
		switch ( m_pCurAnimation->eKind )
		{
			case Anim_Loop:
				this->m_pCurAnimation->tFrame.fIndex = this->m_pCurAnimation->tFrame.fStart;
				break;

			case Anim_ClampForever:
				this->m_pCurAnimation->tFrame.fIndex = this->m_pCurAnimation->tFrame.fMax - 0.1f;
				this->m_bAnimEnd = true;
				break;

		}
	}
}

void CAnimation::UpdateReverseAnim()
{
	this->m_pCurAnimation->tFrame.fIndex -= GET_TIME * this->m_pCurAnimation->tFrame.fCount;

	if ( this->m_pCurAnimation->tFrame.fIndex <= this->m_pCurAnimation->tFrame.fStart )
	{
		switch ( m_pCurAnimation->eKind )
		{
			case Anim_Reverse_Loop:
				this->m_pCurAnimation->tFrame.fIndex = this->m_pCurAnimation->tFrame.fMax - 0.1f;
				break;

			case Anim_Reverse_ClampForever:
				this->m_pCurAnimation->tFrame.fIndex = this->m_pCurAnimation->tFrame.fStart;
				this->m_bAnimEnd = true;
				break;

		}
	}
}
