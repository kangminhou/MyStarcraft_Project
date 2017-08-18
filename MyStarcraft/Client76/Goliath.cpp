#include "stdafx.h"
#include "Goliath.h"

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


CGoliath::CGoliath()
{
}


CGoliath::~CGoliath()
{
}

HRESULT CGoliath::Initialize( void )
{
	this->SetObjKey( L"GoliathtLower" );
	this->m_wstrStateKey = L"Idle";

	/* 유닛의 데이터 초기화.. */
	this->m_tInfoData.fMaxHp = this->m_tInfoData.fCurHp = 125.f;
	this->m_tInfoData.iDefense = 1;
	this->m_tInfoData.fSpeed = Calc_Entity_Speed( 2.64375f );
	//this->m_tInfoData.fSpeed = Calc_Entity_Speed( 10.f );
	this->m_tInfoData.iScope = 8;

	/* 생성 데이터 초기화.. */
	this->m_tGenerateData.fGenerateTime = 1.f;
	this->m_tGenerateData.iRequireMineral = 75;
	this->m_tGenerateData.iRequireGas = 0;
	this->m_tGenerateData.iRequirePopulation = 1;

	/* 유닛 무기 초기화.. */
	this->m_tGroundAttWeapon.pWeapon = m_pWeaponMgr->GetNewWeapon( CWeaponMgr::Weapon_TwinAutocannons );
	this->m_tGroundAttWeapon.byAttackNum = 1;
	this->m_tGroundAttWeapon.fAttRange = 6.f;

	this->m_tAirAttWeapon.pWeapon = m_pWeaponMgr->GetNewWeapon( CWeaponMgr::Weapon_HelfireMissilePack );
	this->m_tAirAttWeapon.byAttackNum = 2;
	this->m_tAirAttWeapon.fAttRange = 5.f;

	RECT tRect = { -8, -9, 8, 10 };
	this->m_tOriginColRect = tRect;

	this->AddComponent( new CMove );

	CUnit::Initialize();

	this->m_pSelectTexture[0] = CTextureMgr::GetInstance()->GetTexture( L"SelectArea", L"Player", 0 );
	this->m_pSelectTexture[1] = CTextureMgr::GetInstance()->GetTexture( L"SelectArea", L"Enemy", 0 );

	return S_OK;
}

int CGoliath::Update( void )
{
	if ( this->m_pCannonAnim )
		this->m_pCannonAnim->UpdateAnim();

	return CUnit::Update();
}

void CGoliath::Render( void )
{
	CUnit::Render();

	/* 그릴 텍스쳐를 찾아냄.. */
	const TEX_INFO* pDrawTexture = NULL;
	const FRAME* pFrame = ((this->m_pCannonAnim) ? NULL : this->m_pCannonAnim->GetCurAnimation());

	if ( pFrame )
		pDrawTexture = this->m_vecDrawCannonTexture[(unsigned int)pFrame->fIndex];

	/* 그림이 중앙이 객체의 좌표가 되도록 설정.. */
	if ( pDrawTexture )
	{
		float fX = pDrawTexture->ImageInfo.Width * 0.5f;
		float fY = pDrawTexture->ImageInfo.Height * 0.5f;
		this->DrawTexture( pDrawTexture, this->m_matCannonWorld, D3DXVECTOR3( fX, fY, 0.f ) );
	}
}

void CGoliath::Release( void )
{
	
}

void CGoliath::InitAnimation()
{
	this->m_pAnimCom->AddAnimation( L"Idle", FRAME( 0.f, 1.f, 1.f, 0.f ), CAnimation::Anim_ClampForever );
	this->m_pAnimCom->AddAnimation( L"Move", FRAME( 0.f, 10.f, 10.f, 0.f ), CAnimation::Anim_Loop );
}

void CGoliath::InitPattern()
{
	this->m_mapPatterns.insert( make_pair( L"Idle", new CPattern_IdleAlert ) );
	this->m_mapPatterns.insert( make_pair( L"Move", new CPattern_Move ) );
	this->m_mapPatterns.insert( make_pair( L"MoveAlert", new CPattern_MoveAlert ) );
	this->m_mapPatterns.insert( make_pair( L"ChaseTarget", new CPattern_ChaseTarget ) );
	this->m_mapPatterns.insert( make_pair( L"AttackTarget", nullptr ) );
	this->m_mapPatterns.insert( make_pair( L"Stop", nullptr ) );
	this->m_mapPatterns.insert( make_pair( L"Attack", new CPattern_Attack ) );
	this->m_mapPatterns.insert( make_pair( L"Die", new CPattern_Die ) );
	this->m_mapPatterns.insert( make_pair( L"Patrol", new CPattern_Patrol ) );
}

void CGoliath::SetPattern( const eGameEntityPattern& _ePatternKind, const bool& _bPrevPattern /*= FALSE */ )
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

			m_wstrStateKey = L"Idle";
			bChangeAnimation = this->m_pAnimCom->ChangeAnimation( m_wstrStateKey.c_str() );

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

			m_wstrStateKey = L"Move";
			bChangeAnimation = this->m_pAnimCom->ChangeAnimation( m_wstrStateKey.c_str() );
			break;

		case CGameEntity::Pattern_MoveAlert:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"MoveAlert" )->second;

			m_wstrStateKey = L"Move";
			bChangeAnimation = this->m_pAnimCom->ChangeAnimation( m_wstrStateKey.c_str() );
			break;

		case CGameEntity::Pattern_Stop:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"Stop" )->second;

			m_wstrStateKey = L"Idle";
			bChangeAnimation = this->m_pAnimCom->ChangeAnimation( m_wstrStateKey.c_str() );
			break;

		case CGameEntity::Pattern_Hold:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"Hold" )->second;

			m_wstrStateKey = L"Idle";
			bChangeAnimation = this->m_pAnimCom->ChangeAnimation( m_wstrStateKey.c_str() );
			break;

		case CGameEntity::Pattern_Patrol:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"Patrol" )->second;

			m_wstrStateKey = L"Move";
			bChangeAnimation = this->m_pAnimCom->ChangeAnimation( m_wstrStateKey.c_str() );
			break;

		case CGameEntity::Pattern_Attack:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"Attack" )->second;

			m_wstrStateKey = L"Idle";
			bChangeAnimation = this->m_pAnimCom->ChangeAnimation( m_wstrStateKey.c_str() );
			break;

		case CGameEntity::Pattern_ChaseTarget:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"ChaseTarget" )->second;

			m_wstrStateKey = L"Move";
			bChangeAnimation = this->m_pAnimCom->ChangeAnimation( m_wstrStateKey.c_str() );
			break;

		case CGameEntity::Pattern_Hit:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"ChaseTarget" )->second;

			m_wstrStateKey = L"Move";
			bChangeAnimation = this->m_pAnimCom->ChangeAnimation( m_wstrStateKey.c_str() );
			break;

		case CGameEntity::Pattern_Die:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"Die" )->second;

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
		this->ChangeDirAnimTexture();
}
