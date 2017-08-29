#include "stdafx.h"
#include "TestUnit.h"

#include "Animation.h"
#include "WeaponMgr.h"

#include "Pattern_Die.h"


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
	this->m_tInfoData.fMaxHp = this->m_tInfoData.fCurHp = 700.f;
	this->m_tInfoData.iDefense = 0;
	this->m_tInfoData.fSpeed = 1.5f;
	this->m_tInfoData.iScope = 7;
	this->m_tInfoData.eArmorUpgradeType = Upgrade_Terran_Infantry_Armor;

	RECT tRect = { -8, -8, 8, 8 };
	this->m_tOriginColRect = tRect;

	/* 유닛 무기 초기화.. */
	this->m_tGroundAttWeapon.pWeapon = m_pWeaponMgr->GetNewWeapon( this->GetObjectType(), CWeaponMgr::Weapon_GaussRifle );
	this->m_tAirAttWeapon.pWeapon = m_pWeaponMgr->GetNewWeapon( this->GetObjectType(), CWeaponMgr::Weapon_GaussRifle );

	CUnit::Initialize();

	return S_OK;
}

int CTestUnit::Update( void )
{
	if ( this->m_bDie )
		return Event_DestoryObject;

	return CUnit::Update();
}

void CTestUnit::Release( void )
{
}

void CTestUnit::SetPattern( const eGameEntityPattern & _ePatternKind, const bool & _bPrevPattern /*= FALSE*/ )
{
	switch ( _ePatternKind )
	{
		case CGameEntity::Pattern_Die:
		{
			this->m_pCurActPattern = this->m_mapPatterns.find( L"Die" )->second;
			this->m_bDie = true;
		}
			break;
	}
}

void CTestUnit::InitAnimation()
{
	this->m_pAnimCom->AddAnimation( L"Idle", FRAME( 0.f, 1.f, 1.f, 0.f ), CAnimation::Anim_Loop );
}

void CTestUnit::InitPattern()
{
	this->m_mapPatterns.insert( make_pair( L"Die", new CPattern_Die ) );
}
