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

	/* ������ ������ �ʱ�ȭ.. */
	this->m_tInfoData.fMaxHp = this->m_tInfoData.fCurHp = 50.f;
	this->m_tInfoData.iDefense = 0;
	this->m_tInfoData.fSpeed = 1.5f;
	this->m_tInfoData.iScope = 7;

	/* ���� ������ �ʱ�ȭ.. */
	this->m_tGenerateData.fGenerateTime = 1.f;
	this->m_tGenerateData.iRequireMineral = 50;
	this->m_tGenerateData.iRequireGas = 0;
	this->m_tGenerateData.iRequirePopulation = 1;

	/* ���� ���� �ʱ�ȭ.. */
	this->m_pGroundAttWeapon = NULL;
	this->m_pAirAttWeapon = NULL;

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

void CTestUnit::SetPattern( const eGameEntityPattern& _ePatternKind )
{
	return;
}

void CTestUnit::InitAnimation()
{
	this->m_pAnimCom->AddAnimation( L"Idle", FRAME( 0.f, 1.f, 1.f, 0.f ), CAnimation::Anim_Loop );
}

void CTestUnit::InitPattern()
{
}