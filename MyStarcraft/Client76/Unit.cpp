#include "stdafx.h"
#include "Unit.h"

#include "TextureMgr.h"
#include "Device.h"

#include "Animation.h"


CUnit::CUnit()
{
}


CUnit::~CUnit()
{
	Release();
}

HRESULT CUnit::Initialize( void )
{
	this->m_wstrStateKey = L"Idle";

	/* 무기 세팅.. */
	//this->m_pGroundAttWeapon = NULL;
	//this->m_pAirAttWeapon = NULL;

	/* 유닛의 데이터 초기화.. */
	this->m_tInfoData.iMaxHp = this->m_tInfoData.iCurHp = 50;
	this->m_tInfoData.iDefense = 0;
	this->m_tInfoData.fSpeed = 1.5f;
	this->m_tInfoData.iScope = 7;

	/* 생성 데이터 초기화.. */
	this->m_tGenerateData.fGenerateTime = 1.f;
	this->m_tGenerateData.iRequireMineral = 50;
	this->m_tGenerateData.iRequireGas = 0;
	this->m_tGenerateData.iRequirePopulation = 1;

	CGameEntity::Initialize();

	return S_OK;
}

int CUnit::Update( void )
{
	int iResult = 0;

	return iResult;
}

void CUnit::Render( void )
{
	const TEX_INFO* pDrawTexture = NULL;
	const FRAME* pCurAnimation = this->m_pAnimCom->GetCurAnimation();

	if ( pCurAnimation && this->m_vecTexture.size() < (size_t)pCurAnimation->fIndex )
		pDrawTexture = this->m_vecTexture[(unsigned int)(pCurAnimation->fIndex)];

	if ( pDrawTexture )
	{
		float fX = pDrawTexture->ImageInfo.Width * 0.5f;
		float fY = pDrawTexture->ImageInfo.Height * 0.5f;

		CDevice::GetInstance()->GetSprite()->Draw(
			pDrawTexture->pTexture,
			NULL,
			&D3DXVECTOR3( fX, fY, 0.f ),
			NULL,
			D3DCOLOR_ARGB( 255, 255, 255, 255 )
		);
	}
}

void CUnit::Release( void )
{
	
}

void CUnit::SetPattern( const eGameEntityPattern& _ePatternKind )
{
	switch ( _ePatternKind )
	{
		case CGameEntity::Pattern_Idle:

			break;

		case CGameEntity::Pattern_Move:
			break;

		case CGameEntity::Pattern_Stop:
			break;

		case CGameEntity::Pattern_Hold:
			break;

		case CGameEntity::Pattern_Patrol:
			break;

		case CGameEntity::Pattern_Attack:
			break;

		default:
			break;

	}

}

void CUnit::InitAnimation()
{
	this->m_pAnimCom->AddAnimation( L"Idle", FRAME( 0.f, 1.f, 1.f, 0.f ), CAnimation::Anim_Loop );
}

void CUnit::InitPattern()
{

}
