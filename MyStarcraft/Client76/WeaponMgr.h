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
		Weapon_FusionCutter,
		Weapon_GaussRifle,
		Weapon_HelfireMissilePack,
		Weapon_TwinAutocannons,
		Weapon_End
	};

private:
	WEAPON_DATA m_tWeaponDataArr[Weapon_End];	// 모든 무기의 초기 값이 세팅될 예정..
	WEAPON_DATA m_tWeaponDataArr2[Weapon_End];	// 모든 무기의 초기 값이 세팅될 예정..

public:
	CWeaponMgr();
	~CWeaponMgr();

public:
	void Initialize();
	void Release();

public:
	CWeapon* GetNewWeapon( const eObjectType& _eObjType, const eWeaponKind& _eWeaponKind );
	void UpgradeWeapon( const eObjectType& _eObjType, const eWeaponUpgradeType& _eWeaponUpgradeType );

private:
	void LoadWeaponData();

};

