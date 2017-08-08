#include "stdafx.h"
#include "Unit.h"

#include "TextureMgr.h"
#include "Device.h"

#include "Animation.h"
#include "EntityPattern.h"
#include "WeaponMgr.h"
#include "Weapon.h"

CWeaponMgr* CUnit::m_pWeaponMgr = NULL;


CUnit::CUnit()
	: m_pGroundAttWeapon(NULL), m_pAirAttWeapon(NULL)
{
	ZeroMemory( &m_tGenerateData, sizeof( UNIT_GENERATE_DATA ) );
}


CUnit::~CUnit()
{
	Release();
}

void CUnit::SetWeaponManager( CWeaponMgr * _pWeaponMgr )
{
	m_pWeaponMgr = _pWeaponMgr;
}

HRESULT CUnit::Initialize( void )
{
	/* 무기 세팅.. */
	//this->m_pGroundAttWeapon = NULL;
	//this->m_pAirAttWeapon = NULL;

	CGameEntity::Initialize();

	return S_OK;
}

int CUnit::Update( void )
{
	int iResult = CGameEntity::Update();

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
	if ( this->m_pGroundAttWeapon )
		safe_delete( this->m_pGroundAttWeapon );

	if ( this->m_pAirAttWeapon )
		safe_delete( this->m_pAirAttWeapon );

	for each (auto pPattern in m_mapPatterns)
	{
		safe_delete( pPattern.second );
	}
}
