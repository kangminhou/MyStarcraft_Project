#include "stdafx.h"
#include "Weapon.h"

#include "GameEntity.h"


CWeapon::CWeapon()
{
}


CWeapon::~CWeapon()
{
	this->Release();
}

void CWeapon::SetAttInterval( const float & _fAttInterval )
{
	if ( this->m_pWeaonData )
		this->m_pWeaonData->fAttInterval = _fAttInterval;
}

void CWeapon::SetWeaponData( WEAPON_DATA * _pWeaponData )
{
	if ( _pWeaponData )
		this->m_pWeaonData = _pWeaponData;
}

const vector<TEX_INFO*>* CWeapon::GetHitTexture() const
{
	if ( this->m_pWeaonData )
		return &this->m_pWeaonData->vecHitTexture;

	return NULL;
}

const WEAPON_DATA * CWeapon::GetWeaponData() const
{
	if ( this->m_pWeaonData )
		return this->m_pWeaonData;

	return NULL;
}

void CWeapon::Initialize()
{
	if ( this->m_pWeaonData )
		m_fRestInterval = this->m_pWeaonData->fAttInterval;
}

void CWeapon::Release()
{
}

void CWeapon::Attack( CGameEntity * _pAttTarget )
{
	
}

bool CWeapon::IsCanAttack()
{
	return ((m_fRestInterval <= 0.f) ? true : false);
}
