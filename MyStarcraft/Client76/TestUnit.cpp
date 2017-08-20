#include "stdafx.h"
#include "TestUnit.h"

#include "Animation.h"
#include "WeaponMgr.h"


CTestUnit::CTestUnit()
{
}


CTestUnit::~CTestUnit()
{
}

HRESULT CTestUnit::Initialize( void )
{
	this->SetObjKey( L"Marine" );
	this->m_wstrStateKey = L"Idle";

	/* 유닛의 데이터 초기화.. */
	this->m_tInfoData.fMaxHp = this->m_tInfoData.fCurHp = 50.f;
	this->m_tInfoData.iDefense = 0;
	this->m_tInfoData.fSpeed = 1.5f;
	this->m_tInfoData.iScope = 7;

	/* 유닛 무기 초기화.. */
	/*this->m_tGroundAttWeapon = NULL;
	this->m_tAirAttWeapon = NULL;*/

	CUnit::Initialize();

	return S_OK;
}

int CTestUnit::Update( void )
{
	CUnit::Update();

	return 0;
}

void CTestUnit::Release( void )
{
}

void CTestUnit::SetPattern( const eGameEntityPattern & _ePatternKind, const bool & _bPrevPattern /*= FALSE*/ )
{
}

void CTestUnit::InitAnimation()
{
	this->m_pAnimCom->AddAnimation( L"Idle", FRAME( 0.f, 1.f, 1.f, 0.f ), CAnimation::Anim_Loop );
}

void CTestUnit::InitPattern()
{
}
