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

	AddComponent( this->m_pGroundAttWeapon );
	AddComponent( this->m_pAirAttWeapon );

	CGameEntity::Initialize();

	return S_OK;
}

int CUnit::Update( void )
{
	int iResult = CGameEntity::Update();

	return iResult;
}

void CUnit::Release( void )
{
}
