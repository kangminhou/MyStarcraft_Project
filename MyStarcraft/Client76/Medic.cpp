#include "stdafx.h"
#include "Medic.h"

#include "WeaponMgr.h"
#include "Move.h"
#include "TextureMgr.h"

#include "Animation.h"
#include "EntityPattern.h"
#include "Pattern_Die.h"
#include "Pattern_Move.h"
#include "Pattern_IdleAlert.h"
#include "Pattern_ChaseTarget.h"
#include "Pattern_MoveAlert.h"
#include "Pattern_HealOurForces.h"


CMedic::CMedic()
{
}


CMedic::~CMedic()
{
}

HRESULT CMedic::Initialize( void )
{
	this->SetObjKey( L"Medic" );
	this->m_wstrStateKey = L"Idle";

	/* 유닛의 데이터 초기화.. */
	this->m_tInfoData.fMaxHp = this->m_tInfoData.fCurHp = 60.f;
	this->m_tInfoData.iDefense = 1;
	this->m_tInfoData.fSpeed = Calc_Entity_Speed(1.5f);
	//this->m_tInfoData.fSpeed = Calc_Entity_Speed( 10.f );
	this->m_tInfoData.iScope = 9;

	/* 생성 데이터 초기화.. */
	this->m_tGenerateData.fGenerateTime = 1.f;
	this->m_tGenerateData.iRequireMineral = 50;
	this->m_tGenerateData.iRequireGas = 25;
	this->m_tGenerateData.iRequirePopulation = 1;

	/* 유닛 무기 초기화.. */
	this->m_tGroundAttWeapon.fAttRange = 1.5f;
	this->m_tGroundAttWeapon.byAttackNum = 1;

	RECT tRect = { -8, -11, 8, 11 };
	this->m_tOriginColRect = tRect;

	this->AddComponent( new CMove );

	CUnit::Initialize();

	this->m_pSelectTexture[0] = CTextureMgr::GetInstance()->GetTexture( L"SelectArea", L"Player", 0 );
	this->m_pSelectTexture[1] = CTextureMgr::GetInstance()->GetTexture( L"SelectArea", L"Enemy", 0 );

	return S_OK;
}

int CMedic::Update( void )
{
	return CUnit::Update();
}

void CMedic::Render( void )
{
	CUnit::Render();
}

void CMedic::Release( void )
{
}

void CMedic::SetPattern( const eGameEntityPattern & _ePatternKind )
{
	if ( m_curActPatternKind == CGameEntity::Pattern_Die )
		return;

	bool bChangeAnimation = false;

	switch ( _ePatternKind )
	{
		case CGameEntity::Pattern_Idle:
		{
			this->m_pCurActPattern = this->m_mapPatterns.find( L"Idle" )->second;
			bChangeAnimation = this->m_pAnimCom->ChangeAnimation( L"Idle" );
			this->m_wstrStateKey = L"Idle";

			//if ( !m_bInitPrevIdlePos )
			//{
			//	m_vPrevIdlePos = this->GetPos();
			//	m_bInitPrevIdlePos = true;
			//}
			//
			//CObjMgr::GetInstance()->FindGameObject<CBackground>()->UpdateUnitPosData( m_vPrevIdlePos, this );
			//m_vPrevIdlePos = this->GetPos();
		}
		break;

		case CGameEntity::Pattern_Move:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"Move" )->second;
			bChangeAnimation = this->m_pAnimCom->ChangeAnimation( L"Move" );
			this->m_wstrStateKey = L"Move";
			break;

		case CGameEntity::Pattern_MoveAlert:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"MoveAlert" )->second;
			bChangeAnimation = this->m_pAnimCom->ChangeAnimation( L"Move" );
			this->m_wstrStateKey = L"Move";
			break;

		case CGameEntity::Pattern_Stop:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"Stop" )->second;
			bChangeAnimation = this->m_pAnimCom->ChangeAnimation( L"Idle" );
			this->m_wstrStateKey = L"Idle";
			break;

		case CGameEntity::Pattern_Hold:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"Hold" )->second;
			bChangeAnimation = this->m_pAnimCom->ChangeAnimation( L"Idle" );
			this->m_wstrStateKey = L"Idle";
			break;

		case CGameEntity::Pattern_Patrol:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"Patrol" )->second;
			bChangeAnimation = this->m_pAnimCom->ChangeAnimation( L"Move" );
			this->m_wstrStateKey = L"Idle";
			break;

		case CGameEntity::Pattern_Attack:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"Attack" )->second;
			bChangeAnimation = this->m_pAnimCom->ChangeAnimation( L"Attack" );
			this->m_wstrStateKey = L"Attack";
			break;

		case CGameEntity::Pattern_ChaseTarget:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"ChaseTarget" )->second;
			bChangeAnimation = this->m_pAnimCom->ChangeAnimation( L"Move" );
			this->m_wstrStateKey = L"Move";
			break;

		case CGameEntity::Pattern_Die:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"Die" )->second;;
			bChangeAnimation = this->m_pAnimCom->ChangeAnimation( L"Die" );
			this->m_wstrStateKey = L"Die";
			break;

		default:
			return;

	}

	if ( this->m_pCurActPattern )
		this->m_pCurActPattern->OnEnable();

	this->m_curActPatternKind = _ePatternKind;

	if ( bChangeAnimation )
		this->ChangeDirAnimTexture();

}

bool CMedic::UseSkill( const eGameEntitySkillKind & wstrSkillName, CGameEntity * _pTarget )
{
	return false;
}

void CMedic::InitAnimation()
{
	this->m_pAnimCom->AddAnimation( L"Idle", FRAME( 0.f, 1.f, 1.f, 0.f ), CAnimation::Anim_Loop );
	this->m_pAnimCom->AddAnimation( L"Move", FRAME( 0.f, 7.f, 7.f, 0.f ), CAnimation::Anim_Loop );
	this->m_pAnimCom->AddAnimation( L"Attack", FRAME( 0.f, 5.f, 2.f, 0.f ), CAnimation::Anim_ClampForever );
	this->m_pAnimCom->AddAnimation( L"Magic", FRAME( 0.f, 10.f, 3.f, 0.f ), CAnimation::Anim_ClampForever );
	this->m_pAnimCom->AddAnimation( L"Die", FRAME( 0.f, 10.f, 9.f, 0.f ), CAnimation::Anim_ClampForever );
}

void CMedic::InitPattern()
{
	this->m_mapPatterns.insert( make_pair( L"Idle", new CPattern_IdleAlert(false) ) );
	this->m_mapPatterns.insert( make_pair( L"Move", new CPattern_Move ) );
	this->m_mapPatterns.insert( make_pair( L"ChaseTarget", new CPattern_ChaseTarget(false) ) );
	this->m_mapPatterns.insert( make_pair( L"MoveAlert", new CPattern_MoveAlert(false) ) );
	this->m_mapPatterns.insert( make_pair( L"Attack", new CPattern_HealOurForces) );
	this->m_mapPatterns.insert( make_pair( L"Stop", nullptr) );
	this->m_mapPatterns.insert( make_pair( L"Die", new CPattern_Die ) );
}
