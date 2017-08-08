#include "stdafx.h"
#include "Marine.h"

#include "Animation.h"
#include "WeaponMgr.h"

#include "Pattern_IdleAlert.h"
#include "Pattern_Move.h"
#include "Pattern_MoveAlert.h"


CMarine::CMarine()
{
}


CMarine::~CMarine()
{
}

HRESULT CMarine::Initialize( void )
{
	this->m_wstrStateKey = L"Idle";

	/* 유닛의 데이터 초기화.. */
	this->m_tInfoData.fMaxHp = this->m_tInfoData.fCurHp = 50.f;
	this->m_tInfoData.iDefense = 0;
	this->m_tInfoData.fSpeed = 1.5f;
	this->m_tInfoData.iScope = 7;

	/* 생성 데이터 초기화.. */
	this->m_tGenerateData.fGenerateTime = 1.f;
	this->m_tGenerateData.iRequireMineral = 50;
	this->m_tGenerateData.iRequireGas = 0;
	this->m_tGenerateData.iRequirePopulation = 1;

	/* 유닛 무기 초기화.. */
	this->m_pGroundAttWeapon = m_pWeaponMgr->GetNewWeapon( CWeaponMgr::Weapon_GaussRifle );
	this->m_pAirAttWeapon = m_pWeaponMgr->GetNewWeapon( CWeaponMgr::Weapon_GaussRifle );

	CUnit::Initialize();

	return S_OK;
}

int CMarine::Update( void )
{
	CUnit::Update();

	return 0;
}

void CMarine::Release( void )
{
}

void CMarine::SetPattern( const eGameEntityPattern& _ePatternKind )
{
	bool bChangeAnimation = false;

	switch ( _ePatternKind )
	{
		case CGameEntity::Pattern_Idle:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"Idle" )->second;
			bChangeAnimation = this->m_pAnimCom->ChangeAnimation( L"Idle" );
			break;

		case CGameEntity::Pattern_Move:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"Move" )->second;
			bChangeAnimation = this->m_pAnimCom->ChangeAnimation( L"Move" );
			break;

		case CGameEntity::Pattern_Stop:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"Stop" )->second;
			bChangeAnimation = this->m_pAnimCom->ChangeAnimation( L"Idle" );
			break;

		case CGameEntity::Pattern_Hold:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"Hold" )->second;
			bChangeAnimation = this->m_pAnimCom->ChangeAnimation( L"Idle" );
			break;

		case CGameEntity::Pattern_Patrol:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"Patrol" )->second;
			bChangeAnimation = this->m_pAnimCom->ChangeAnimation( L"Move" );
			break;

		case CGameEntity::Pattern_Attack:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"MoveAlert" )->second;
			bChangeAnimation = this->m_pAnimCom->ChangeAnimation( L"MoveAlert" );
			break;

		case CGameEntity::Pattern_AttackTarget:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"AttackTarget" )->second;
			bChangeAnimation = this->m_pAnimCom->ChangeAnimation( L"Move" );
			break;
			
		case CGameEntity::Pattern_Hit:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"Hit" )->second;
			break;

		default:
			return;

	}

	if ( bChangeAnimation )
		this->ChangeDirAnimTexture();
	this->m_curActPatternKind = _ePatternKind;

}

void CMarine::InitAnimation()
{
	this->m_pAnimCom->AddAnimation( L"Idle", FRAME( 0.f, 1.f, 1.f, 0.f ), CAnimation::Anim_Loop );
}

void CMarine::InitPattern()
{
	this->m_mapPatterns.insert( make_pair( L"Idle", new CPattern_IdleAlert ) );
	this->m_mapPatterns.insert( make_pair( L"Move", new CPattern_Move ) );
	this->m_mapPatterns.insert( make_pair( L"MoveAlert", new CPattern_MoveAlert ) );
	this->m_mapPatterns.insert( make_pair( L"ChaseTarget", nullptr ) );
	this->m_mapPatterns.insert( make_pair( L"Attack", nullptr ) );
	this->m_mapPatterns.insert( make_pair( L"Hit", nullptr ) );
}
