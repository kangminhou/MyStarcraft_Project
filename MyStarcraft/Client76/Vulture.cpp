#include "stdafx.h"
#include "Vulture.h"

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


CVulture::CVulture()
{
}


CVulture::~CVulture()
{
}

HRESULT CVulture::Initialize( void )
{
	/* Add Sound To SoundVector.. */
	this->AddSound( L"tvurdy00.wav", CUnit::Sound_Born );

	this->AddSound( L"tvuyes00.wav", CUnit::Sound_ActPattern );
	this->AddSound( L"tvuyes01.wav", CUnit::Sound_ActPattern );
	this->AddSound( L"tvuyes02.wav", CUnit::Sound_ActPattern );
	this->AddSound( L"tvuyes03.wav", CUnit::Sound_ActPattern );

	this->AddSound( L"tvuwht00.wav", CUnit::Sound_Click );
	this->AddSound( L"tvuwht01.wav", CUnit::Sound_Click );
	this->AddSound( L"tvuwht02.wav", CUnit::Sound_Click );
	this->AddSound( L"tvuwht03.wav", CUnit::Sound_Click );

	this->AddSound( L"tvudth00.wav", CUnit::Sound_Death );


	this->SetObjKey( L"Vulture" );
	this->m_wstrStateKey = L"All"; 

	this->m_wstrFaceKey = L"FaceVulture";
	this->m_wstrWireFrameKey = L"Vulture";

	this->m_byFaceFrameNum = 45;

	/* 유닛의 데이터 초기화.. */
	this->m_tInfoData.fMaxHp = this->m_tInfoData.fCurHp = 80.f;
	this->m_tInfoData.iDefense = 0;
	this->m_tInfoData.fSpeed = Calc_Entity_Speed(3.75f);
	//this->m_tInfoData.fSpeed = Calc_Entity_Speed( 10.f );
	this->m_tInfoData.iScope = 8;
	this->m_tInfoData.nDefenceIconFrame = 293;

	this->m_vecEntityInformation.push_back( CGameEntity::Entity_Mechanic );
	
	/* 유닛 무기 초기화.. */
	this->m_tGroundAttWeapon.pWeapon = m_pWeaponMgr->GetNewWeapon( this->GetObjectType(), CWeaponMgr::Weapon_FragmentationGrenade );
	this->m_tGroundAttWeapon.byAttackNum = 1;
	this->m_tGroundAttWeapon.fAttRange = 5.f;

	RECT tRect = { -16, -16, 16, 16 };
	this->m_tOriginColRect = tRect;

	this->AddComponent( new CMove );

	CUnit::Initialize();

	this->m_pSelectTexture[0] = CTextureMgr::GetInstance()->GetTexture( L"SelectArea", L"Player", 0 );
	this->m_pSelectTexture[1] = CTextureMgr::GetInstance()->GetTexture( L"SelectArea", L"Enemy", 0 );

	return S_OK;
}

int CVulture::Update( void )
{
	return CUnit::Update();
}

void CVulture::Render( void )
{
	CUnit::Render();
}

void CVulture::Release( void )
{
	
}

void CVulture::InitAnimation()
{
	this->m_pAnimCom->AddAnimation( L"All", FRAME( 0.f, 1.f, 1.f, 0.f ), CAnimation::Anim_ClampForever );
}

void CVulture::InitPattern()
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

void CVulture::SetPattern( const eGameEntityPattern& _ePatternKind, const bool& _bPrevPattern /*= FALSE */ )
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
			break;

		case CGameEntity::Pattern_MoveAlert:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"MoveAlert" )->second;
			break;

		case CGameEntity::Pattern_Stop:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"Stop" )->second;
			break;

		case CGameEntity::Pattern_Hold:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"Hold" )->second;
			break;

		case CGameEntity::Pattern_Patrol:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"Patrol" )->second;
			break;

		case CGameEntity::Pattern_Attack:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"Attack" )->second;
			//this->m_pCurActPattern = nullptr;
			break;

		case CGameEntity::Pattern_ChaseTarget:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"ChaseTarget" )->second;
			break;

		case CGameEntity::Pattern_Hit:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"ChaseTarget" )->second;
			break;

		case CGameEntity::Pattern_Die:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"Die" )->second;;

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

bool CVulture::UseSkill( const eGameEntitySkillKind& _eSkillKind, CGameEntity* _pTarget )
{
	return true;
}
