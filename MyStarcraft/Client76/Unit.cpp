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
	return CGameEntity::Update();
}

void CUnit::Release( void )
{
}
