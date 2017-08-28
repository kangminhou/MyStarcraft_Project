#include "stdafx.h"
#include "Medic.h"

#include "WeaponMgr.h"
#include "Move.h"
#include "TextureMgr.h"
#include "TimeMgr.h"

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
	/* Add Sound To SoundVector.. */
	this->AddSound( L"TMdRdy00.wav", CUnit::Sound_Born );

	this->AddSound( L"tmdyes00.wav", CUnit::Sound_ActPattern );
	this->AddSound( L"TMdYes01.wav", CUnit::Sound_ActPattern );
	this->AddSound( L"TMdYes02.wav", CUnit::Sound_ActPattern );
	this->AddSound( L"tmdyes03.wav", CUnit::Sound_ActPattern );

	this->AddSound( L"tmdwht00.wav", CUnit::Sound_Click );
	this->AddSound( L"TMdWht01.wav", CUnit::Sound_Click );
	this->AddSound( L"TMdWht02.wav", CUnit::Sound_Click );
	this->AddSound( L"TMdWht03.wav", CUnit::Sound_Click );

	this->AddSound( L"TMdDth00.wav", CUnit::Sound_Death );

	this->SetObjKey( L"Medic" );
	this->m_wstrStateKey = L"Idle";

	this->m_wstrFaceKey = L"FaceMedic";
	this->m_wstrWireFrameKey = L"Medic";

	this->m_byFaceFrameNum = 45;

	/* 유닛의 데이터 초기화.. */
	this->m_tInfoData.fMaxHp = this->m_tInfoData.fCurHp = 60.f;
	this->m_tInfoData.iDefense = 1;
	this->m_tInfoData.fSpeed = Calc_Entity_Speed(1.5f);
	//this->m_tInfoData.fSpeed = Calc_Entity_Speed( 10.f );
	this->m_tInfoData.iScope = 9;
	this->m_tInfoData.nDefenceIconFrame = 292;

	/* 유닛 무기 초기화.. */
	this->m_tGroundAttWeapon.fAttRange = 1.5f;
	this->m_tGroundAttWeapon.byAttackNum = 1;

	RECT tRect = { -8, -8, 8, 8 };
	this->m_tOriginColRect = tRect;

	this->AddComponent( new CMove );

	CUnit::Initialize();

	this->m_pSelectTexture[0] = CTextureMgr::GetInstance()->GetTexture( L"SelectArea", L"Player", 0 );
	this->m_pSelectTexture[1] = CTextureMgr::GetInstance()->GetTexture( L"SelectArea", L"Enemy", 0 );

	this->m_bUseDeathEffect = false;

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

void CMedic::SetPattern( const eGameEntityPattern & _ePatternKind, const bool & _bPrevPattern /*= FALSE*/ )
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
			bChangeAnimation = this->m_pAnimCom->ChangeAnimation( L"Work" );
			this->m_wstrStateKey = L"Work";
			break;

		case CGameEntity::Pattern_MoveAlert:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"MoveAlert" )->second;
			bChangeAnimation = this->m_pAnimCom->ChangeAnimation( L"Work" );
			this->m_wstrStateKey = L"Work";
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
			bChangeAnimation = this->m_pAnimCom->ChangeAnimation( L"Work" );
			this->m_wstrStateKey = L"Work";
			break;

		case CGameEntity::Pattern_Attack:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"Attack" )->second;
			bChangeAnimation = this->m_pAnimCom->ChangeAnimation( L"Attack" );
			this->m_wstrStateKey = L"Attack";
			break;

		case CGameEntity::Pattern_ChaseTarget:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"ChaseTarget" )->second;
			bChangeAnimation = this->m_pAnimCom->ChangeAnimation( L"Work" );
			this->m_wstrStateKey = L"Work";
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
		this->ChangeLookAnimTexture();

}

bool CMedic::UseSkill( const eGameEntitySkillKind & _eSkillKind, CGameEntity * _pTarget )
{
	switch ( _eSkillKind )
	{
		case CGameEntity::Skill_Heal:
		{
			_pTarget->SetCurHp( _pTarget->GetCurHp() + 10.f * CTimeMgr::GetInstance()->GetTime() ); 
		}
		break;

		default:
			return false;
	}

	return true;
}

void CMedic::InitAnimation()
{
	this->m_pAnimCom->AddAnimation( L"Idle", FRAME( 0.f, 1.f, 1.f, 0.f ), CAnimation::Anim_Loop );
	this->m_pAnimCom->AddAnimation( L"Work", FRAME( 0.f, 7.f, 7.f, 0.f ), CAnimation::Anim_Loop );
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
