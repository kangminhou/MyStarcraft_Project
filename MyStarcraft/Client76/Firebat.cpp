#include "stdafx.h"
#include "Firebat.h"

#include "Animation.h"
#include "WeaponMgr.h"
#include "Device.h"
#include "TimeMgr.h"
#include "TextureMgr.h"
#include "KeyMgr.h"
#include "ObjMgr.h"

#include "Pattern_IdleAlert.h"
#include "Pattern_Move.h"
#include "Pattern_MoveAlert.h"
#include "Pattern_ChaseTarget.h"
#include "Pattern_Attack.h"
#include "Pattern_Die.h"
#include "Pattern_Patrol.h"
#include "Move.h"
#include "Background.h"
#include "Weapon.h"
#include "Corps.h"


CFirebat::CFirebat()
	: m_bUseSteampack(false)
	, m_fSteampackDuration(0.f)
{
}


CFirebat::~CFirebat()
{
}

HRESULT CFirebat::Initialize( void )
{
	/* Add Sound To SoundVector.. */
	this->AddSound( L"tfbrdy00.wav", CUnit::Sound_Born );

	this->AddSound( L"tfbyes00.wav", CUnit::Sound_ActPattern );
	this->AddSound( L"tfbyes01.wav", CUnit::Sound_ActPattern );
	this->AddSound( L"tfbyes02.wav", CUnit::Sound_ActPattern );
	this->AddSound( L"tfbyes03.wav", CUnit::Sound_ActPattern );

	this->AddSound( L"tfbwht00.wav", CUnit::Sound_Click );
	this->AddSound( L"tfbwht01.wav", CUnit::Sound_Click );
	this->AddSound( L"tfbwht02.wav", CUnit::Sound_Click );
	this->AddSound( L"tfbwht03.wav", CUnit::Sound_Click );

	this->AddSound( L"tfbdth00.wav", CUnit::Sound_Death );
	this->AddSound( L"tfbdth01.wav", CUnit::Sound_Death );
	this->AddSound( L"tfbdth02.wav", CUnit::Sound_Death );

	this->AddSound( L"tmasti00.wav", CUnit::Sound_ETC );
	this->AddSound( L"tmasti01.wav", CUnit::Sound_ETC );


	this->SetObjKey( L"Firebat" );
	this->m_wstrStateKey = L"Idle";

	this->m_wstrFaceKey = L"FaceFirebat";
	this->m_wstrWireFrameKey = L"Firebat";

	this->m_byFaceFrameNum = 45;

	/* 유닛의 데이터 초기화.. */
	this->m_tInfoData.fMaxHp = this->m_tInfoData.fCurHp = 50.f;
	this->m_tInfoData.iDefense = 1;
	this->m_tInfoData.fSpeed = Calc_Entity_Speed(2.25f);
	//this->m_tInfoData.fSpeed = Calc_Entity_Speed( 10.f );
	this->m_tInfoData.iScope = 7;
	this->m_tInfoData.nDefenceIconFrame = 292; 

	this->m_vecEntityInformation.push_back( CGameEntity::Entity_Bionic );

	/* 유닛 무기 초기화.. */
	this->m_tGroundAttWeapon.pWeapon = m_pWeaponMgr->GetNewWeapon( this->GetObjectType(), CWeaponMgr::Weapon_FlameThrower );
	this->m_tGroundAttWeapon.byAttackNum = 2;
	this->m_tGroundAttWeapon.fAttRange = 1.f;

	RECT tRect = { -8, -8, 8, 8 };
	this->m_tOriginColRect = tRect;

	this->AddComponent( new CMove );

	CUnit::Initialize();

	this->m_pSelectTexture[0] = CTextureMgr::GetInstance()->GetTexture( L"SelectArea", L"Player", 0 );
	this->m_pSelectTexture[1] = CTextureMgr::GetInstance()->GetTexture( L"SelectArea", L"Enemy", 0 );

	return S_OK;
}

int CFirebat::Update( void )
{
	if ( this->m_bUseSteampack )
	{
		this->m_fSteampackDuration -= GET_TIME;

		if ( this->m_fSteampackDuration <= 0.f )
		{
			this->m_tGroundAttWeapon.pWeapon->SetAttInterval( 1.f );

			this->m_tInfoData.fSpeed = Calc_Entity_Speed(1.5f);

			this->m_pAnimCom->SetAnimationData( L"Attack", FRAME( 0.f, 10.f, 3.f, 0.f ) );

			this->m_bUseSteampack = false;
		}
	}

	return CUnit::Update();
}

void CFirebat::Release( void )
{
	
}

void CFirebat::Render( void )
{
	CUnit::Render();

	//static float fAnim = 0.f;
	//fAnim += CTimeMgr::GetInstance()->GetTime() * 2.f;
	//const TEX_INFO*	pTex = CTextureMgr::GetInstance()->GetTexture( L"Effect", L"Firebat", ((int)fAnim) + m_byDirAnimIndex * 12 );
	//
	//this->DrawTexture(
	//	pTex,
	//	this->GetWorldMatrix(),
	//	D3DXVECTOR3( pTex->ImageInfo.Width * 0.5f, pTex->ImageInfo.Height * 0.5f, 0.f )
	//);
}

void CFirebat::SetPattern( const eGameEntityPattern & _ePatternKind, const bool & _bPrevPattern /*= FALSE*/ )
{
	if ( m_curActPatternKind == CGameEntity::Pattern_Die )
		return;

	if ( _ePatternKind == CGameEntity::Pattern_Idle ||
		 _ePatternKind == CGameEntity::Pattern_MoveAlert )
		this->m_vecActPatterns.push_back( _ePatternKind );

	if ( this->m_vecActPatterns.size() > 10 )
		this->m_vecActPatterns.erase( this->m_vecActPatterns.begin() );

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

		case CGameEntity::Pattern_Hit:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"ChaseTarget" )->second;
			bChangeAnimation = this->m_pAnimCom->ChangeAnimation( L"Work" );
			this->m_wstrStateKey = L"Work";
			break;

		case CGameEntity::Pattern_Die:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"Die" )->second;;
			bChangeAnimation = this->m_pAnimCom->ChangeAnimation( L"Die" );
			this->m_wstrStateKey = L"Die";

			if ( this->m_pEntityBelongToCorps )
				this->m_pEntityBelongToCorps->EraseUnit( this );
			break;

		default:
			return;

	}

	if ( this->m_pCurActPattern )
	{
		this->m_pCurActPattern->SetPrevPattern( _bPrevPattern );
		this->m_pCurActPattern->OnEnable();
	}

	this->m_curActPatternKind = _ePatternKind;

	if ( bChangeAnimation )
		this->ChangeLookAnimTexture();
}

bool CFirebat::UseSkill( const eGameEntitySkillKind& _eSkillKind, CGameEntity* _pTarget )
{
	switch ( _eSkillKind )
	{
		case CGameEntity::Skill_SteamPack:
		{
			if ( this->m_tInfoData.fCurHp <= 10.f )
				return false;

			if ( !m_bUseSteampack )
			{
				this->m_tGroundAttWeapon.pWeapon->SetAttInterval( 0.7f );

				this->m_tInfoData.fSpeed *= 1.4f;

				this->m_pAnimCom->SetAnimationData( L"Attack", FRAME( 0.f, 14.f, 3.f, 0.f ) );

				this->m_bUseSteampack = true;
			}

			this->m_tInfoData.fCurHp -= 10.f;
			this->m_fSteampackDuration = 15.f;
		}
		break;

		default:
			return false;

	}

	return true;
}

void CFirebat::InitAnimation()
{
	this->m_pAnimCom->AddAnimation( L"Idle", FRAME( 0.f, 1.f, 1.f, 0.f ), CAnimation::Anim_Loop );
	this->m_pAnimCom->AddAnimation( L"Work", FRAME( 0.f, 9.f, 8.f, 0.f ), CAnimation::Anim_Loop );
	this->m_pAnimCom->AddAnimation( L"Attack", FRAME( 0.f, 1.f, 1.f, 0.f ), CAnimation::Anim_ClampForever );
}

void CFirebat::InitPattern()
{
	this->m_mapPatterns.insert( make_pair( L"Idle", new CPattern_IdleAlert ) );
	this->m_mapPatterns.insert( make_pair( L"Move", new CPattern_Move ) );
	this->m_mapPatterns.insert( make_pair( L"MoveAlert", new CPattern_MoveAlert ) );
	this->m_mapPatterns.insert( make_pair( L"ChaseTarget", new CPattern_ChaseTarget ) );
	this->m_mapPatterns.insert( make_pair( L"AttackTarget", nullptr) );
	this->m_mapPatterns.insert( make_pair( L"Stop", nullptr) );
	this->m_mapPatterns.insert( make_pair( L"Attack", new CPattern_Attack ) );
	this->m_mapPatterns.insert( make_pair( L"Die", new CPattern_Die ) );
	this->m_mapPatterns.insert( make_pair( L"Patrol", new CPattern_Patrol) );
}
