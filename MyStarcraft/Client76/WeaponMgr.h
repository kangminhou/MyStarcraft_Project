#pragma once
#include "Include.h"

class CWeapon;
class CWeaponMgr
{
public:
	enum eWeaponKind
	{
		Weapon_ArcliteCannon,
		Weapon_ArcliteShockCannon,
		Weapon_FlameThrower,
		Weapon_FragmentationGrenade,
		Weapon_GaussRifle,
		Weapon_HelfireMissilePack,
		Weapon_TwinAutocannons,
		Weapon_End
	};

private:
	WEAPON_DATA m_tWeaponDataArr[Weapon_End];	// 모든 무기의 초기 값이 세팅될 예정..

public:
	CWeaponMgr();
	~CWeaponMgr();

	void Initialize();
	CWeapon* GetNewWeapon( const eWeaponKind& _eWeaponKind );

private:
	void LoadWeaponData();

};

