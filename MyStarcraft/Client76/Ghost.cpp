#include "stdafx.h"
#include "Ghost.h"

#include "Animation.h"
#include "WeaponMgr.h"
#include "Device.h"
#include "TimeMgr.h"
#include "TextureMgr.h"
#include "KeyMgr.h"
#include "ObjMgr.h"
#include "Mouse.h"
#include "EffectMgr.h"

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


CGhost::CGhost()
	: m_bUseNuclear(false)
{
}


CGhost::~CGhost()
{
}

void CGhost::NuclearEnd()
{
	m_bUseNuclear = false;
}

HRESULT CGhost::Initialize( void )
{
	/* Add Sound To SoundVector.. */
	this->AddSound( L"tghrdy00.wav", CUnit::Sound_Born );

	this->AddSound( L"tghyes00.wav", CUnit::Sound_ActPattern );
	this->AddSound( L"tghyes01.wav", CUnit::Sound_ActPattern );
	this->AddSound( L"tghyes02.wav", CUnit::Sound_ActPattern );
	this->AddSound( L"tghyes03.wav", CUnit::Sound_ActPattern );

	this->AddSound( L"tghwht00.wav", CUnit::Sound_Click );
	this->AddSound( L"tghwht01.wav", CUnit::Sound_Click );
	this->AddSound( L"tghwht02.wav", CUnit::Sound_Click );
	this->AddSound( L"tghwht03.wav", CUnit::Sound_Click );

	this->AddSound( L"tghdth00.wav", CUnit::Sound_Death );

	this->AddSound( L"tghdth00.wav", CUnit::Sound_ETC );
	this->AddSound( L"tghlas00.wav", CUnit::Sound_ETC );
	this->AddSound( L"tadupd04.wav", CUnit::Sound_ETC );

	this->SetObjKey( L"Ghost" );
	this->m_wstrStateKey = L"Idle";

	this->m_wstrFaceKey = L"FaceGhost";
	this->m_wstrWireFrameKey = L"Ghost";

	this->m_byFaceFrameNum = 45;

	/* 유닛의 데이터 초기화.. */
	this->m_tInfoData.fMaxHp = this->m_tInfoData.fCurHp = 45.f;
	this->m_tInfoData.iDefense = 0;
	this->m_tInfoData.fSpeed = Calc_Entity_Speed(2.25f);
	//this->m_tInfoData.fSpeed = Calc_Entity_Speed( 10.f );
	this->m_tInfoData.iScope = 9;
	this->m_tInfoData.nDefenceIconFrame = 292;
	this->m_tInfoData.eArmorUpgradeType = Upgrade_Terran_Infantry_Armor;

	this->m_vecEntityInformation.push_back( CGameEntity::Entity_Bionic );
	this->m_vecEntityInformation.push_back( CGameEntity::Entity_MagicUnit );

	/* 유닛 무기 초기화.. */
	this->m_tGroundAttWeapon.pWeapon = m_pWeaponMgr->GetNewWeapon( this->GetObjectType(), CWeaponMgr::Weapon_C10_ConcussionRifle );
	this->m_tGroundAttWeapon.byAttackNum = 1;
	this->m_tGroundAttWeapon.fAttRange = 7.f;

	this->m_tAirAttWeapon.pWeapon = m_pWeaponMgr->GetNewWeapon( this->GetObjectType(), CWeaponMgr::Weapon_C10_ConcussionRifle );
	this->m_tAirAttWeapon.byAttackNum = 1;
	this->m_tAirAttWeapon.fAttRange = 7.f;

	RECT tRect = { -8, -8, 8, 8 };
	this->m_tOriginColRect = tRect;

	this->AddComponent( new CMove );

	CUnit::Initialize();

	this->m_pSelectTexture[0] = CTextureMgr::GetInstance()->GetTexture( L"SelectArea", L"Player", 0 );
	this->m_pSelectTexture[1] = CTextureMgr::GetInstance()->GetTexture( L"SelectArea", L"Enemy", 0 );

	this->m_bUseDeathEffect = false;

	this->m_pEffectMgr = CEffectMgr::GetInstance();

	this->m_color = D3DCOLOR_ARGB( 255, 255, 255, 255 );

	return S_OK;
}

int CGhost::Update( void )
{
	return CUnit::Update();
}

void CGhost::Render( void )
{
	/* 그릴 텍스쳐를 찾아냄.. */
	const TEX_INFO* pDrawTexture = NULL;
	const FRAME* pCurAnimation = this->m_pAnimCom->GetCurAnimation();

	size_t u = (size_t)pCurAnimation->fIndex;
	if ( pCurAnimation && this->m_vecTexture.size() > (size_t)pCurAnimation->fIndex )
		pDrawTexture = this->m_vecTexture[(unsigned int)(pCurAnimation->fIndex)];

	/* 그림이 중앙이 객체의 좌표가 되도록 설정.. */
	if ( pDrawTexture )
	{
		float fX = pDrawTexture->ImageInfo.Width * 0.5f;
		float fY = pDrawTexture->ImageInfo.Height * 0.5f;
		this->DrawTexture( pDrawTexture, this->GetWorldMatrix(), D3DXVECTOR3( fX, fY, 0.f ), this->m_color );
	}

	if ( this->m_bDrawHpBarUI )
	{
		this->RenderHpUI();
	}

	RECT rcDraw = { (LONG)(this->m_tColRect.left - m_vScroll.x), (LONG)(this->m_tColRect.top - m_vScroll.y),
		(LONG)(this->m_tColRect.right - m_vScroll.x), (LONG)(this->m_tColRect.bottom - m_vScroll.y) };

	this->DrawRect( rcDraw );
}

void CGhost::Release( void )
{
}

void CGhost::SetPattern( const eGameEntityPattern& _ePatternKind, const bool& _bPrevPattern /*= FALSE*/ )
{
	if ( m_curActPatternKind == CGameEntity::Pattern_Die )
		return;

	if ( _ePatternKind != CGameEntity::Pattern_Die && m_bUseNuclear )
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
			//this->m_pCurActPattern = nullptr;
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
			this->m_pCurActPattern = this->m_mapPatterns.find( L"Die" )->second;
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

bool CGhost::UseSkill( const eGameEntitySkillKind& _eSkillKind, CGameEntity* _pTarget )
{
	switch ( _eSkillKind )
	{
		case CGameEntity::Skill_Clocking:
		{
			if ( this->m_bClocking )
			{
				this->m_bClocking = false;
				this->m_color = D3DCOLOR_ARGB( 255, 255, 255, 255 );
			}
			else
			{
				this->m_bClocking = true;
				this->m_color = D3DCOLOR_ARGB( 128, 255, 255, 255 );
			}
		}
		break;
		
		case CGameEntity::SKill_LockDown:
		{
			//this->m_pMouse->GetPos() + m_vScroll
			vector<CGameEntity*> vecEntity;
			m_tDragData.vStartPos = m_tDragData.vEndPos = this->m_pMouse->GetPos() + m_vScroll;

			for ( int i = OBJ_TYPE_USER; i <= OBJ_TYPE_USER2; ++i )
			{
				this->m_pObjMgr->CheckDragEntitys( vecEntity, m_tDragData, (eObjectType)i );

				for ( size_t j = 0; j < vecEntity.size(); ++j )
				{
					if ( vecEntity[j]->GetCheckUnitInformation( CGameEntity::Entity_Mechanic ) )
					{
						m_pEffectMgr->ShowLockDown( this, vecEntity[j] );
						this->SoundPlay( CGameEntity::Sound_ETC, 0 );
						return true;
					}
				}
			}
		}
		break;

		case CGameEntity::Skill_Nuclear:
		{
			m_bUseNuclear = true;

			this->SetPattern( CGameEntity::Pattern_Idle );

			D3DXVECTOR3 vFirePos = this->m_pMouse->GetPos() + m_vScroll;
			this->m_pEffectMgr->ShowNuclear( this, vFirePos );

			this->LookPos( vFirePos );
			this->ChangeAnimation( L"Attack" );
		}
			break;

		default:
			return false;

	}

	return true;
}

void CGhost::InitAnimation()
{
	this->m_pAnimCom->AddAnimation( L"Idle", FRAME( 0.f, 1.f, 1.f, 0.f ), CAnimation::Anim_Loop );
	this->m_pAnimCom->AddAnimation( L"Work", FRAME( 0.f, 9.f, 9.f, 0.f ), CAnimation::Anim_Loop );
	this->m_pAnimCom->AddAnimation( L"Attack", FRAME( 0.f, 10.f, 3.f, 0.f ), CAnimation::Anim_ClampForever );
	this->m_pAnimCom->AddAnimation( L"Die", FRAME( 0.f, 10.f, 8.f, 0.f ), CAnimation::Anim_ClampForever );
}

void CGhost::InitPattern()
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