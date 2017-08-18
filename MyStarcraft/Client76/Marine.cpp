#include "stdafx.h"
#include "Marine.h"

#include "Include.h"

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


CMarine::CMarine()
{
}


CMarine::~CMarine()
{
	Release();
}

HRESULT CMarine::Initialize( void )
{
	this->SetObjKey( L"Marine" );
	this->m_wstrStateKey = L"Idle";

	/* 유닛의 데이터 초기화.. */
	this->m_tInfoData.fMaxHp = this->m_tInfoData.fCurHp = 40.f;
	this->m_tInfoData.iDefense = 0;
	this->m_tInfoData.fSpeed = Calc_Entity_Speed(1.5f);
	//this->m_tInfoData.fSpeed = Calc_Entity_Speed( 10.f );
	this->m_tInfoData.iScope = 7;

	/* 생성 데이터 초기화.. */
	this->m_tGenerateData.fGenerateTime = 1.f;
	this->m_tGenerateData.iRequireMineral = 50;
	this->m_tGenerateData.iRequireGas = 0;
	this->m_tGenerateData.iRequirePopulation = 1;

	/* 유닛 무기 초기화.. */
	this->m_tGroundAttWeapon.pWeapon = m_pWeaponMgr->GetNewWeapon( CWeaponMgr::Weapon_GaussRifle );
	this->m_tGroundAttWeapon.byAttackNum = 1;
	this->m_tGroundAttWeapon.fAttRange = 4.f;

	this->m_tAirAttWeapon.pWeapon = m_pWeaponMgr->GetNewWeapon( CWeaponMgr::Weapon_GaussRifle );
	this->m_tAirAttWeapon.byAttackNum = 1;
	this->m_tAirAttWeapon.fAttRange = 4.f;

	RECT tRect = { -8, -9, 8, 10 };
	this->m_tOriginColRect = tRect;

	this->AddComponent( new CMove );

	CUnit::Initialize();

	this->m_pSelectTexture[0] = CTextureMgr::GetInstance()->GetTexture( L"SelectArea", L"Player", 0 );
	this->m_pSelectTexture[1] = CTextureMgr::GetInstance()->GetTexture( L"SelectArea", L"Enemy", 0 );

	return S_OK;
}

int CMarine::Update( void )
{
	//if ( CKeyMgr::GetInstance()->GetKeyOnceDown( VK_RETURN ) || CKeyMgr::GetInstance()->GetKeyOnceDown( VK_SPACE ) )
	//{
	//	int iPlus = ((CKeyMgr::GetInstance()->GetKeyOnceDown( VK_RETURN ) ? 1 : -1));
	//
	//	if ( CKeyMgr::GetInstance()->GetKeyStayDown( 'A' ) )
	//	{
	//		this->m_tOriginColRect.left += iPlus;
	//	}
	//	else if ( CKeyMgr::GetInstance()->GetKeyStayDown( 'D' ) )
	//	{
	//		this->m_tOriginColRect.right += iPlus;
	//	}
	//	else if ( CKeyMgr::GetInstance()->GetKeyStayDown( 'W' ) )
	//	{
	//		this->m_tOriginColRect.top += iPlus;
	//	}
	//	else if ( CKeyMgr::GetInstance()->GetKeyStayDown( 'S' ) )
	//	{
	//		this->m_tOriginColRect.bottom += iPlus;
	//	}
	//}

	return CUnit::Update();
}

void CMarine::Render( void )
{
	CUnit::Render();

	//TCHAR str[128] = L"";
	//swprintf_s( str, L"%f, %f, %f", this->GetTransform()->GetPos().x, this->GetTransform()->GetPos().y, this->GetTransform()->GetPos().z );
	//
	//D3DXMATRIX matTrans;
	//D3DXMatrixTranslation( &matTrans, 100.f, 200.f, 0.f );
	//CDevice::GetInstance()->GetSprite()->SetTransform( &matTrans );
	//
	//CDevice::GetInstance()->GetFont()->DrawTextW(
	//	CDevice::GetInstance()->GetSprite(),
	//	str,
	//	lstrlen( str ),
	//	NULL,
	//	NULL,
	//	D3DCOLOR_ARGB( 255, 255, 255, 255 )
	//);
}

void CMarine::Release( void )
{
}

void CMarine::SetPattern( const eGameEntityPattern& _ePatternKind, const bool& _bPrevPattern /*= FALSE*/ )
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
			//this->m_pCurActPattern = this->m_mapPatterns.find( L"Attack" )->second;
			this->m_pCurActPattern = nullptr;
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
		this->ChangeDirAnimTexture();

}

void CMarine::InitAnimation()
{
	this->m_pAnimCom->AddAnimation( L"Idle", FRAME( 0.f, 1.f, 1.f, 0.f ), CAnimation::Anim_Loop );
	this->m_pAnimCom->AddAnimation( L"Work", FRAME( 0.f, 9.f, 9.f, 0.f ), CAnimation::Anim_Loop );
	this->m_pAnimCom->AddAnimation( L"Attack", FRAME( 0.f, 10.f, 3.f, 0.f ), CAnimation::Anim_ClampForever );
	this->m_pAnimCom->AddAnimation( L"Die", FRAME( 0.f, 10.f, 8.f, 0.f ), CAnimation::Anim_ClampForever );
}

void CMarine::InitPattern()
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
	//this->m_mapPatterns.insert( make_pair( L"Die", new CPattern_Die ) );
}
