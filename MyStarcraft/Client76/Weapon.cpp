#include "stdafx.h"
#include "Weapon.h"

#include "GameEntity.h"
#include "TimeMgr.h"


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

void CWeapon::SetWeaponOwner( CGameEntity * _pOwnerEntity )
{
	this->m_pOwnerEntity = _pOwnerEntity;
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
	this->m_fRestInterval = 0.f;
}

void CWeapon::Release()
{
}

void CWeapon::Attack( CGameEntity * _pAttTarget )
{
	this->m_fRestInterval = this->m_pWeaonData->fAttInterval;

	NormalAttack( _pAttTarget );
}

bool CWeapon::AttackCoolTimeUpdate()
{
	if ( !this->IsCanAttack() )
	{
		this->m_fRestInterval -= GET_TIME;
		return false;
	}

	return true;
}

bool CWeapon::IsCanAttack()
{
	return ((m_fRestInterval <= 0.f) ? true : false);
}

void CWeapon::NormalAttack( CGameEntity * _pAttTarget )
{
	_pAttTarget->HitEntity( this->m_pOwnerEntity, this->m_pWeaonData->fDamage );
}

void CWeapon::NormalSplashAttack( CGameEntity * _pAttTarget )
{
	_pAttTarget->HitEntity( this->m_pOwnerEntity, this->m_pWeaonData->fDamage );
}

void CWeapon::CircleSplashAttack( CGameEntity * _pAttTarget )
{
	_pAttTarget->HitEntity( this->m_pOwnerEntity, this->m_pWeaonData->fDamage );
}
