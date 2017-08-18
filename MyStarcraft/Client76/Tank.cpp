#include "stdafx.h"
#include "Tank.h"

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


CTank::CTank()
{
}


CTank::~CTank()
{
	this->Release();
}

HRESULT CTank::Initialize( void )
{
	this->SetObjKey( L"TankLower" );
	this->m_wstrStateKey = L"Idle"; 

	/* 유닛의 데이터 초기화.. */
	this->m_tInfoData.fMaxHp = this->m_tInfoData.fCurHp = 80.f;
	this->m_tInfoData.iDefense = 0;
	this->m_tInfoData.fSpeed = Calc_Entity_Speed(3.75f);
	//this->m_tInfoData.fSpeed = Calc_Entity_Speed( 10.f );
	this->m_tInfoData.iScope = 8;

	/* 생성 데이터 초기화.. */
	this->m_tGenerateData.fGenerateTime = 1.f;
	this->m_tGenerateData.iRequireMineral = 75;
	this->m_tGenerateData.iRequireGas = 0;
	this->m_tGenerateData.iRequirePopulation = 1;

	/* 유닛 무기 초기화.. */
	this->m_tNormalTankGroundAttData.pWeapon = m_pWeaponMgr->GetNewWeapon( CWeaponMgr::Weapon_ArcliteCannon );
	this->m_tNormalTankGroundAttData.byAttackNum = 1;
	this->m_tNormalTankGroundAttData.fAttRange = 7.f;
	this->m_tNormalTankGroundAttData.pWeapon->SetWeaponOwner( this );

	this->m_tSiegeTankGroundAttData.pWeapon = m_pWeaponMgr->GetNewWeapon( CWeaponMgr::Weapon_ArcliteShockCannon );
	this->m_tSiegeTankGroundAttData.byAttackNum = 1;
	this->m_tSiegeTankGroundAttData.fAttRange = 12.f;
	this->m_tSiegeTankGroundAttData.pWeapon->SetWeaponOwner( this );

	this->m_tGroundAttWeapon = this->m_tNormalTankGroundAttData;

	this->m_pCannonAnim = new CAnimation;
	this->m_pCannonAnim->Initialize();

	RECT tRect = { -8, -9, 8, 10 };
	this->m_tOriginColRect = tRect;

	this->AddComponent( new CMove );

	CUnit::Initialize();

	this->m_pSelectTexture[0] = CTextureMgr::GetInstance()->GetTexture( L"SelectArea", L"Player", 0 );
	this->m_pSelectTexture[1] = CTextureMgr::GetInstance()->GetTexture( L"SelectArea", L"Enemy", 0 );

	for ( int i = 0; i < 17; ++i )
	{
		this->m_vecCannonTexture[0].push_back( CTextureMgr::GetInstance()->GetTexture( L"TankUpper", L"All", i ) );
		this->m_vecCannonTexture[1].push_back( CTextureMgr::GetInstance()->GetTexture( L"STankUpper", L"Attack", i ) );
	}

	for ( int i = 0; i < 5; ++i )
	{
		this->m_vecCannonCangeMotionTexture.push_back( CTextureMgr::GetInstance()->GetTexture( L"STankUpper", L"Idle", i ) );
	}

	this->m_bSiegeMode = false;
	this->m_bChangeMode = false;
	this->m_iModeChangeLevel = 0;

	return S_OK;
}

#include "Mouse.h"
int CTank::Update( void )
{
	if ( CKeyMgr::GetInstance()->GetKeyOnceDown( 'O' ) )
	{
		this->UseSkill( CGameEntity::Skill_SiegeOnOff, NULL );
	}

	if ( this->m_pCannonAnim )
		this->m_pCannonAnim->UpdateAnim();

	if ( this->m_bChangeMode )
	{
		this->UpdateModeChange();
	}

	static float fX = -14.f;
	static float fY = -10.f;

	if ( CKeyMgr::GetInstance()->GetKeyStayDown( 'J' ) )
	{
		fX -= 10.f * CTimeMgr::GetInstance()->GetTime();
	}
	if ( CKeyMgr::GetInstance()->GetKeyStayDown( 'L' ) )
	{
		fX += 10.f * CTimeMgr::GetInstance()->GetTime();
	}
	if ( CKeyMgr::GetInstance()->GetKeyStayDown( 'I' ) )
	{
		fY -= 10.f * CTimeMgr::GetInstance()->GetTime();
	}
	if ( CKeyMgr::GetInstance()->GetKeyStayDown( 'K' ) )
	{
		fY += 10.f * CTimeMgr::GetInstance()->GetTime();
	}
	if ( CKeyMgr::GetInstance()->GetKeyStayDown( 'M' ) )
	{
		D3DXVECTOR3 vMouse = CMouse::GetInstance()->GetPos() + m_vScroll;
		this->LookPos( vMouse );
	}

	return CUnit::Update();
}

void CTank::Render( void )
{
	CUnit::Render();

	/*
	 * 탱크 캐논 그리기.. 
	 */
	if ( this->m_bSiegeMode || this->m_bChangeMode )
	{
		D3DXMATRIX matTrans, matScale;
		D3DXMatrixTranslation( &matTrans, -9.f, -16.f, 0.f );
		D3DXMatrixScaling( &matScale, ((this->GetTransform()->GetLook().x < 0.f) ? -1.f : 1.f), 1.f, 1.f );
		this->m_matCannonWorld = matScale * matTrans * this->GetWorldMatrix();
	}
	else
	{
		this->m_matCannonWorld = this->GetWorldMatrix();
	}

	/* 그릴 텍스쳐를 찾아냄.. */
	const TEX_INFO* pDrawTexture = NULL;

	if ( !this->m_bChangeMode )
	{
		pDrawTexture = this->m_vecCannonTexture[this->m_bSiegeMode][this->m_byLookAnimIndex];
	}
	else
	{
		const FRAME* pCurAnimation = this->m_pCannonAnim->GetCurAnimation();

		if ( pCurAnimation )
			pDrawTexture = this->m_vecCannonCangeMotionTexture[(unsigned int)pCurAnimation->fIndex];
	}

	/* 그림이 중앙이 객체의 좌표가 되도록 설정.. */
	if ( pDrawTexture )
	{
		float fX = pDrawTexture->ImageInfo.Width * 0.5f;
		float fY = pDrawTexture->ImageInfo.Height * 0.5f;
		this->DrawTexture( pDrawTexture, this->m_matCannonWorld, D3DXVECTOR3( fX, fY, 0.f ) );
	}
	else
	{
		int a = 10;
		return;
	}
}

void CTank::Release( void )
{
	safe_delete( m_pCannonAnim );
}

void CTank::InitAnimation()
{
	this->m_pAnimCom->AddAnimation( L"Idle", FRAME( 0.f, 1.f, 1.f, 0.f ), CAnimation::Anim_ClampForever );
	this->m_pAnimCom->AddAnimation( L"Move", FRAME( 0.f, 3.f, 3.f, 0.f ), CAnimation::Anim_ClampForever );
	this->m_pAnimCom->AddAnimation( L"Change_Normal_to_Siege", FRAME( 0.f, 6.f, 6.f, 0.f ), CAnimation::Anim_ClampForever );
	this->m_pAnimCom->AddAnimation( L"Change_Siege_to_Normal", FRAME( 0.f, 6.f, 6.f, 0.f ), CAnimation::Anim_Reverse_ClampForever );

	this->m_pCannonAnim->AddAnimation( L"Normal_Attack", FRAME( 0.f, 3.f, 1.f, 0.f ), CAnimation::Anim_ClampForever );
	this->m_pCannonAnim->AddAnimation( L"Change_Normal_to_Siege", FRAME( 0.f, 5.f, 5.f, 0.f ), CAnimation::Anim_ClampForever );
	this->m_pCannonAnim->AddAnimation( L"Change_Siege_to_Normal", FRAME( 0.f, 5.f, 5.f, 0.f ), CAnimation::Anim_Reverse_ClampForever );
	this->m_pCannonAnim->AddAnimation( L"Normal_Attack", FRAME( 0.f, 3.f, 1.f, 0.f ), CAnimation::Anim_ClampForever );
}

void CTank::InitPattern()
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

void CTank::SetPattern( const eGameEntityPattern& _ePatternKind, const bool& _bPrevPattern /*= FALSE */ )
{
	if ( this->m_bChangeMode )
		return;

	if ( m_curActPatternKind == CGameEntity::Pattern_Die )
		return;

	if ( this->m_bSiegeMode &&
		(_ePatternKind != CGameEntity::Pattern_Idle && _ePatternKind != CGameEntity::Pattern_Attack &&
		 _ePatternKind != CGameEntity::Pattern_Stop && _ePatternKind != CGameEntity::Pattern_Die))
	{
		return;
	}

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

			if ( !this->m_bSiegeMode )
			{
				m_wstrStateKey = L"Idle";
				bChangeAnimation = this->m_pAnimCom->ChangeAnimation( m_wstrStateKey.c_str() );
			}
		}
		break;

		case CGameEntity::Pattern_Move:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"Move" )->second;

			if ( !this->m_bSiegeMode )
			{
				m_wstrStateKey = L"Move";
				bChangeAnimation = this->m_pAnimCom->ChangeAnimation( m_wstrStateKey.c_str() );
			}
			break;

		case CGameEntity::Pattern_MoveAlert:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"MoveAlert" )->second;

			if ( !this->m_bSiegeMode )
			{
				m_wstrStateKey = L"Move";
				bChangeAnimation = this->m_pAnimCom->ChangeAnimation( m_wstrStateKey.c_str() );
			}
			break;

		case CGameEntity::Pattern_Stop:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"Stop" )->second;

			if ( !this->m_bSiegeMode )
			{
				m_wstrStateKey = L"Idle";
				bChangeAnimation = this->m_pAnimCom->ChangeAnimation( m_wstrStateKey.c_str() );
			}
			break;

		case CGameEntity::Pattern_Hold:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"Hold" )->second;

			if ( !this->m_bSiegeMode )
			{
				m_wstrStateKey = L"Idle";
				bChangeAnimation = this->m_pAnimCom->ChangeAnimation( m_wstrStateKey.c_str() );
			}
			break;

		case CGameEntity::Pattern_Patrol:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"Patrol" )->second;

			if ( !this->m_bSiegeMode )
			{
				m_wstrStateKey = L"Move";
				bChangeAnimation = this->m_pAnimCom->ChangeAnimation( m_wstrStateKey.c_str() );
			}
			break;

		case CGameEntity::Pattern_Attack:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"Attack" )->second;

			if ( !this->m_bSiegeMode )
			{
				m_wstrStateKey = L"Idle";
				bChangeAnimation = this->m_pAnimCom->ChangeAnimation( m_wstrStateKey.c_str() );
			}
			break;

		case CGameEntity::Pattern_ChaseTarget:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"ChaseTarget" )->second;

			if ( !this->m_bSiegeMode )
			{
				m_wstrStateKey = L"Move";
				bChangeAnimation = this->m_pAnimCom->ChangeAnimation( m_wstrStateKey.c_str() );
			}
			break;

		case CGameEntity::Pattern_Hit:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"ChaseTarget" )->second;

			if ( !this->m_bSiegeMode )
			{
				m_wstrStateKey = L"Move";
				bChangeAnimation = this->m_pAnimCom->ChangeAnimation( m_wstrStateKey.c_str() );
			}
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

bool CTank::UseSkill( const eGameEntitySkillKind& _eSkillKind, CGameEntity* _pTarget )
{
	switch ( _eSkillKind )
	{
		case Skill_SiegeOnOff:
			this->SetPattern( CGameEntity::Pattern_Idle );
			this->m_bChangeMode = true;
			this->m_bSiegeMode = ((this->m_bSiegeMode) ? false : true);
			break;
	}

	return true;
}

void CTank::UpdateModeChange()
{
	if ( this->m_bSiegeMode )
	{
		switch ( this->m_iModeChangeLevel )
		{
			case 0:
			{
				D3DXVECTOR3 vLook, vDir;
				D3DXVec3Normalize( &vLook, &D3DXVECTOR3( 1.f, -1.f, 0.f ) );
				this->SetLook( vLook );

				this->m_tGroundAttWeapon = this->m_tSiegeTankGroundAttData;
				this->m_bInfluenceDir = false;

				this->m_pAnimCom->ChangeAnimation( L"Change_Normal_to_Siege" );

				this->m_pCannonAnim->ChangeAnimation( L"Change_Normal_to_Siege" );
				this->m_pCannonAnim->PauseAnim();

				this->SetObjKey( L"STankLower" );
				this->m_wstrStateKey = L"All";
				this->SetSize( 1.f, 1.f );
				this->ChangeDirAnimTexture();

				this->m_iModeChangeLevel = 1;
			}
				break;
			case 1:
				if ( this->m_pAnimCom->GetIsAnimEnd() )
				{
					this->m_pCannonAnim->StopPauseAnim();

					this->m_iModeChangeLevel = 2;
				}
				break;
			case 2:
				if ( this->m_pCannonAnim->GetIsAnimEnd() )
				{
					this->m_pAnimCom->PauseAnim();

					this->ModeChangeEnd();
				}
				break;
		}
	}
	else
	{
		switch ( this->m_iModeChangeLevel )
		{
			case 0:
			{
				this->m_pAnimCom->StopPauseAnim();
				this->m_tGroundAttWeapon = this->m_tNormalTankGroundAttData;

				this->m_pAnimCom->ChangeAnimation( L"Change_Siege_to_Normal" );
				this->m_pAnimCom->PauseAnim();

				this->m_pCannonAnim->ChangeAnimation( L"Change_Siege_to_Normal" );

				this->m_iModeChangeLevel = 1;
			}
				break;
			case 1:
				if ( this->m_pCannonAnim->GetIsAnimEnd() )
				{
					this->m_pAnimCom->StopPauseAnim();
					this->m_iModeChangeLevel = 2;
				}
				break;
			case 2:
				if ( this->m_pAnimCom->GetIsAnimEnd() )
				{
					D3DXVECTOR3 vLook, vDir;
					D3DXVec3Normalize( &vLook, &D3DXVECTOR3( 1.f, 1.f, 0.f ) );
					this->SetDir( vLook );
					this->SetLook( vLook );

					this->m_bInfluenceDir = true;

					this->SetObjKey( L"TankLower" );
					this->m_wstrStateKey = L"Idle";
					this->m_pAnimCom->ChangeAnimation( L"Idle" );
					this->UpdateDirAnimIndex();

					this->ModeChangeEnd();
				}
				break;
		}
	}
	/* 탱크 노말 -> 시즈 모드 */
	// 1. Siege Tank Lower Animation 실행..
	// 2. 1번 작업이 끝나면 Siege Tank Upper Idle Animation 실행..
	// 3. 2번 작업이 끝나면 Idle 패턴으로..

	/* 탱크 시즈 -> 노말 모드 */
	// 1. Siege Tank Upper Idle Animation 거꾸로 실행..
	// 2. Siege Tank Lower Animation 거꾸로 실행..
	// 3. 2번 작업이 끝나면 Idle 패턴으로..

}

void CTank::ModeChangeEnd()
{
	this->m_bChangeMode = false;
	this->m_iModeChangeLevel = 0;
}
