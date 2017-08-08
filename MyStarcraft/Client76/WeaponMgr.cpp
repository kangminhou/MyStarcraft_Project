#include "stdafx.h"
#include "WeaponMgr.h"

#include "Weapon.h"


CWeaponMgr::CWeaponMgr()
{
}


CWeaponMgr::~CWeaponMgr()
{
}

void CWeaponMgr::Initialize()
{
	/* 무기의 정보 초기화.. ( XML 데이터로 처리할 예정???? ) */
	this->m_tWeaponDataArr[Weapon_GaussRifle].eUpgradeType = Upgrade_Terran_Infantry_Weapons;
	this->m_tWeaponDataArr[Weapon_GaussRifle].fAttInterval = Calc_Weapon_Interval( 22.f );
	this->m_tWeaponDataArr[Weapon_GaussRifle].fDamage = 6.f;
	this->m_tWeaponDataArr[Weapon_GaussRifle].iUpgradePlus = 1;
	this->m_tWeaponDataArr[Weapon_GaussRifle].iMinAttRange = 0;
	this->m_tWeaponDataArr[Weapon_GaussRifle].iMaxAttRange = 8;

	this->m_tWeaponDataArr[Weapon_GaussRifle].bSplash = false;
	this->m_tWeaponDataArr[Weapon_GaussRifle].iInside = 0;
	this->m_tWeaponDataArr[Weapon_GaussRifle].iMiddle = 0;
	this->m_tWeaponDataArr[Weapon_GaussRifle].iOutSide = 0;
}

CWeapon * CWeaponMgr::GetNewWeapon( eWeaponKind eWeaponKind )
{
	CWeapon* pOutWeapon = nullptr;

	switch ( eWeaponKind )
	{
		case CWeaponMgr::Weapon_GaussRifle:
			pOutWeapon = new CWeapon;
			pOutWeapon->SetWeaponData( &this->m_tWeaponDataArr[Weapon_GaussRifle] );
			break;
	}

	return pOutWeapon;
}
