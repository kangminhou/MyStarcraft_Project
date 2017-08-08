#include "stdafx.h"
#include "Weapon.h"


CWeapon::CWeapon()
	: m_pHitTexture(NULL)
{
	ZeroMemory( &this->m_tWeaonData, sizeof( WEAPON_DATA ) );
}


CWeapon::~CWeapon()
{
	this->Release();
}

void CWeapon::SetAttInterval( const float & _fAttInterval )
{
	this->m_tWeaonData.fAttInterval = _fAttInterval;
}

const TEX_INFO * CWeapon::GetHitTexture() const
{
	return this->m_pHitTexture;
}

const WEAPON_DATA * CWeapon::GetWeaponData()
{
	return &this->m_tWeaonData;
}

void CWeapon::Release()
{
}

bool CWeapon::IsCanAttack()
{
	return ((m_fRestInterval <= 0.f) ? true : false);
}
