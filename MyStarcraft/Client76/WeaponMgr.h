#pragma once
#include "Include.h"

class CWeapon;
class CWeaponMgr
{
public:
	enum eWeaponKind
	{
		Weapon_GaussRifle,
		Weapon_End
	};

private:
	WEAPON_DATA m_tWeaponDataArr[Weapon_End];	// ��� ������ �ʱ� ���� ���õ� ����..

public:
	CWeaponMgr();
	~CWeaponMgr();

	void Initialize();
	CWeapon* GetNewWeapon( eWeaponKind eWeaponKind );

};

