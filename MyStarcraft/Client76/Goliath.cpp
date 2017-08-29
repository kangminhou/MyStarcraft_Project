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
	: m_wstrCannonObjKey(L"")
	, m_wstrCannonStateKey(L"")
{
}


CGoliath::~CGoliath()
{
	Release();
}

HRESULT CGoliath::Initialize( void )
{
	/* Add Sound To SoundVector.. */
	this->AddSound( L"tgordy00.wav", CUnit::Sound_Born );

	this->AddSound( L"tgoyes00.wav", CUnit::Sound_ActPattern );
	this->AddSound( L"tgoyes01.wav", CUnit::Sound_ActPattern );
	this->AddSound( L"tgoyes02.wav", CUnit::Sound_ActPattern );
	this->AddSound( L"tgoyes03.wav", CUnit::Sound_ActPattern );

	this->AddSound( L"tgowht00.wav", CUnit::Sound_Click );
	this->AddSound( L"tgowht01.wav", CUnit::Sound_Click );
	this->AddSound( L"tgowht02.wav", CUnit::Sound_Click );
	this->AddSound( L"tgowht03.wav", CUnit::Sound_Click );

	this->AddSound( L"tgodth00.wav", CUnit::Sound_Death );

	this->SetObjKey( L"GoliathtUpper" );
	this->m_wstrStateKey = L"Idle";

	this->m_wstrFaceKey = L"FaceGoliath";
	this->m_wstrWireFrameKey = L"Goliatht";

	this->m_byFaceFrameNum = 30;

	/* 유닛의 데이터 초기화.. */
	this->m_tInfoData.fMaxHp = this->m_tInfoData.fCurHp = 125.f;
	this->m_tInfoData.iDefense = 1;
	this->m_tInfoData.fSpeed = Calc_Entity_Speed( 2.64375f );
	//this->m_tInfoData.fSpeed = Calc_Entity_Speed( 10.f );
	this->m_tInfoData.iScope = 8;
	this->m_tInfoData.nDefenceIconFrame = 293;

	this->m_vecEntityInformation.push_back( CGameEntity::Entity_Mechanic );

	/* 유닛 무기 초기화.. */
	this->m_tGroundAttWeapon.pWeapon = m_pWeaponMgr->GetNewWeapon( this->GetObjectType(), CWeaponMgr::Weapon_TwinAutocannons );
	this->m_tGroundAttWeapon.byAttackNum = 1;
	this->m_tGroundAttWeapon.fAttRange = 6.f;

	this->m_tAirAttWeapon.pWeapon = m_pWeaponMgr->GetNewWeapon( this->GetObjectType(), CWeaponMgr::Weapon_HelfireMissilePack );
	this->m_tAirAttWeapon.byAttackNum = 2;
	this->m_tAirAttWeapon.fAttRange = 5.f;

	this->m_pCannonAnim = new CAnimation;
	this->m_pCannonAnim->Initialize();

	RECT tRect = { -16, -16, 16, 16 };
	this->m_tOriginColRect = tRect;

	this->AddComponent( new CMove );

	CUnit::Initialize();

	this->m_pSelectTexture[0] = CTextureMgr::GetInstance()->GetTexture( L"SelectArea", L"Player", 0 );
	this->m_pSelectTexture[1] = CTextureMgr::GetInstance()->GetTexture( L"SelectArea", L"Enemy", 0 );

	this->m_bUseDirAnimIndex = true;

	m_wstrCannonObjKey = L"GoliathtLower";
	m_wstrCannonStateKey = L"Idle";
	this->m_pCannonAnim->ChangeAnimation( m_wstrCannonStateKey.c_str() );

	this->UpdateLookAnimIndex();
	this->ChangeDirAnimIndex();
	this->ChangeLookAnimTexture();

	return S_OK;
}

int CGoliath::Update( void )
{
	if ( this->m_pCannonAnim )
		this->m_pCannonAnim->UpdateAnim();

	//static float fX = -14.f;
	//static float fY = -10.f;
	//
	//if ( CKeyMgr::GetInstance()->GetKeyStayDown( 'J' ) )
	//{
	//	fX -= 10.f * CTimeMgr::GetInstance()->GetTime();
	//}
	//if ( CKeyMgr::GetInstance()->GetKeyStayDown( 'L' ) )
	//{
	//	fX += 10.f * CTimeMgr::GetInstance()->GetTime();
	//}
	//if ( CKeyMgr::GetInstance()->GetKeyStayDown( 'I' ) )
	//{
	//	fY -= 10.f * CTimeMgr::GetInstance()->GetTime();
	//}
	//if ( CKeyMgr::GetInstance()->GetKeyStayDown( 'K' ) )
	//{
	//	fY += 10.f * CTimeMgr::GetInstance()->GetTime();
	//}
	//if ( CKeyMgr::GetInstance()->GetKeyStayDown( 'M' ) )
	//{
	//	D3DXVECTOR3 vMouse = CMouse::GetInstance()->GetPos() + m_vScroll;
	//	this->LookPos( vMouse );
	//}

	this->m_matCannonWorld = this->GetWorldMatrix();

	return CUnit::Update();
}

void CGoliath::Render( void )
{
	/* 그릴 텍스쳐를 찾아냄.. */
	const TEX_INFO* pDrawTexture = NULL;
	const FRAME* pCurAnimation = NULL;

	if ( this->m_pCannonAnim )
	{
		pCurAnimation = this->m_pCannonAnim->GetCurAnimation();
		if ( pCurAnimation && this->m_vecTrunkTexture.size() > (unsigned int)pCurAnimation->fIndex )
			pDrawTexture = this->m_vecTrunkTexture[(unsigned int)pCurAnimation->fIndex];

		/* 그림이 중앙이 객체의 좌표가 되도록 설정.. */
		if ( pDrawTexture )
		{
			float fX = pDrawTexture->ImageInfo.Width * 0.5f;
			float fY = pDrawTexture->ImageInfo.Height * 0.5f;
			this->DrawTexture( pDrawTexture, this->GetWorldMatrix(), D3DXVECTOR3( fX, fY, 0.f ) );
		}

		/* 그릴 텍스쳐를 찾아냄.. */
		pDrawTexture = NULL;
		if ( this->m_pAnimCom )
		{
			pCurAnimation = this->m_pAnimCom->GetCurAnimation();

			if ( pCurAnimation && this->m_vecTexture.size() > (size_t)pCurAnimation->fIndex )
				pDrawTexture = this->m_vecTexture[(unsigned int)(pCurAnimation->fIndex)];

			/* 그림이 중앙이 객체의 좌표가 되도록 설정.. */
			if ( pDrawTexture )
			{
				float fX = pDrawTexture->ImageInfo.Width * 0.5f;
				float fY = pDrawTexture->ImageInfo.Height * 0.5f;
				this->DrawTexture( pDrawTexture, this->m_matCannonWorld, D3DXVECTOR3( fX, fY, 0.f ) );
			}
		}
	}

	//RECT rcDraw = { (LONG)(this->m_tColRect.left - m_vScroll.x), (LONG)(this->m_tColRect.top - m_vScroll.y),
	//	(LONG)(this->m_tColRect.right - m_vScroll.x), (LONG)(this->m_tColRect.bottom - m_vScroll.y) };
	//
	//this->DrawRect( rcDraw );
}

void CGoliath::Release( void )
{
	safe_delete( m_pCannonAnim );
}

void CGoliath::InitAnimation()
{
	this->m_pAnimCom->AddAnimation( L"Idle", FRAME( 0.f, 1.f, 1.f, 0.f ), CAnimation::Anim_Loop );
	this->m_pAnimCom->AddAnimation( L"Work", FRAME( 0.f, 10.f, 10.f, 0.f ), CAnimation::Anim_Loop );
	this->m_pAnimCom->AddAnimation( L"Attack", FRAME( 0.f, 3.f, 1.f, 0.f ), CAnimation::Anim_ClampForever );

	this->m_pCannonAnim->AddAnimation( L"Idle", FRAME( 0.f, 1.f, 1.f, 0.f ), CAnimation::Anim_Loop );
	this->m_pCannonAnim->AddAnimation( L"Work", FRAME( 0.f, 10.f, 10.f, 0.f ), CAnimation::Anim_Loop );
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

void CGoliath::ChangeDirAnimIndex()
{
	const FRAME* pTempCurFrame = this->m_pCannonAnim->GetCurAnimation();
	if ( pTempCurFrame )
	{
		m_vecTrunkTexture.clear();

		int iAnimLength = int( pTempCurFrame->fMax );
		int iStart = ((m_bInfluenceDir) ? iAnimLength * this->m_byLookAnimIndex : 0);
		int iEnd = iAnimLength + iStart;

		//cout << "iStart : " << iStart << ", iAnimLength : " << iAnimLength << ", iEnd : " << iEnd << endl;

		for ( ; iStart < iEnd; ++iStart )
		{
			const TEX_INFO* pAnimTex = CTextureMgr::GetInstance()->GetTexture(
				this->m_wstrCannonObjKey.c_str(), m_wstrCannonStateKey.c_str(), iStart );

			m_vecTrunkTexture.push_back( pAnimTex );
		}
	}
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

			this->m_wstrStateKey = L"Idle";
			this->m_wstrCannonStateKey = L"Idle";

			if ( this->m_pAnimCom->ChangeAnimation( this->m_wstrStateKey.c_str() ) )
				bChangeAnimation = true;
			if ( this->m_pCannonAnim->ChangeAnimation( m_wstrCannonStateKey.c_str() ) )
				bChangeAnimation = true;
		}
		break;

		case CGameEntity::Pattern_Move:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"Move" )->second;

			this->m_wstrStateKey = L"Work";
			this->m_wstrCannonStateKey = L"Work";

			if ( this->m_pAnimCom->ChangeAnimation( this->m_wstrStateKey.c_str() ) )
				bChangeAnimation = true;
			if ( this->m_pCannonAnim->ChangeAnimation( m_wstrCannonStateKey.c_str() ) )
				bChangeAnimation = true;
			
			break;

		case CGameEntity::Pattern_MoveAlert:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"MoveAlert" )->second;

			this->m_wstrStateKey = L"Work";
			this->m_wstrCannonStateKey = L"Work";

			if ( this->m_pAnimCom->ChangeAnimation( this->m_wstrStateKey.c_str() ) )
				bChangeAnimation = true;
			if ( this->m_pCannonAnim->ChangeAnimation( m_wstrCannonStateKey.c_str() ) )
				bChangeAnimation = true;
			break;

		case CGameEntity::Pattern_Stop:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"Stop" )->second;

			this->m_wstrStateKey = L"Idle";
			this->m_wstrCannonStateKey = L"Idle";

			if ( this->m_pAnimCom->ChangeAnimation( this->m_wstrStateKey.c_str() ) )
				bChangeAnimation = true;
			if ( this->m_pCannonAnim->ChangeAnimation( m_wstrCannonStateKey.c_str() ) )
				bChangeAnimation = true;
			break;

		case CGameEntity::Pattern_Hold:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"Hold" )->second;

			this->m_wstrStateKey = L"Idle";
			this->m_wstrCannonStateKey = L"Idle";

			if ( this->m_pAnimCom->ChangeAnimation( this->m_wstrStateKey.c_str() ) )
				bChangeAnimation = true;
			if ( this->m_pCannonAnim->ChangeAnimation( m_wstrCannonStateKey.c_str() ) )
				bChangeAnimation = true;
			break;

		case CGameEntity::Pattern_Patrol:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"Patrol" )->second;

			this->m_wstrStateKey = L"Work";
			this->m_wstrCannonStateKey = L"Work";

			if ( this->m_pAnimCom->ChangeAnimation( this->m_wstrStateKey.c_str() ) )
				bChangeAnimation = true;
			if ( this->m_pCannonAnim->ChangeAnimation( m_wstrCannonStateKey.c_str() ) )
				bChangeAnimation = true;
			break;

		case CGameEntity::Pattern_Attack:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"Attack" )->second;

			this->m_wstrStateKey = L"Attack";
			this->m_wstrCannonStateKey = L"Idle";

			if ( this->m_pAnimCom->ChangeAnimation( this->m_wstrStateKey.c_str() ) )
				bChangeAnimation = true;
			if ( this->m_pCannonAnim->ChangeAnimation( m_wstrCannonStateKey.c_str() ) )
				bChangeAnimation = true;
			break;

		case CGameEntity::Pattern_ChaseTarget:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"ChaseTarget" )->second;

			this->m_wstrStateKey = L"Work";
			this->m_wstrCannonStateKey = L"Work";

			if ( this->m_pAnimCom->ChangeAnimation( this->m_wstrStateKey.c_str() ) )
				bChangeAnimation = true;
			if ( this->m_pCannonAnim->ChangeAnimation( m_wstrCannonStateKey.c_str() ) )
				bChangeAnimation = true;
			break;

		case CGameEntity::Pattern_Hit:
			this->m_pCurActPattern = this->m_mapPatterns.find( L"ChaseTarget" )->second;

			this->m_wstrStateKey = L"Work";
			this->m_wstrCannonStateKey = L"Work";

			if ( this->m_pAnimCom->ChangeAnimation( this->m_wstrStateKey.c_str() ) )
				bChangeAnimation = true;
			if ( this->m_pCannonAnim->ChangeAnimation( m_wstrCannonStateKey.c_str() ) )
				bChangeAnimation = true;
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
		this->ChangeLookAnimTexture();
}
