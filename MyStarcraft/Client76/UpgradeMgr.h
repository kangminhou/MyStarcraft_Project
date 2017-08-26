#pragma once
#include "Include.h"

class CGameEntity;
class CWeaponMgr;

class CUpgradeMgr
{
	DECLARE_SINGLETON(CUpgradeMgr)

private:
	pair<list<CGameEntity*>, BYTE> m_ArmorUpgradeDataArr[2][Upgrade_Armor_End];
	CWeaponMgr*	m_pWeaponMgr;

public:
	CUpgradeMgr();
	~CUpgradeMgr();

public:
	void SetWeaponMgr( CWeaponMgr* _pWeaponMgr );

public:
	void Initialize();

public:
	void AddArmorUpgradeObserver( CGameEntity* _pObserver, const eArmorUpgradeType& _eUpgradeType );
	void EraseArmorUpgradeObserver( CGameEntity* _pObserver, const eArmorUpgradeType& _eUpgradeType );

	void UpgradeWeapon( const eObjectType& _eObjType, const eWeaponUpgradeType& _eUpgradeType );
	void UpgradeArmor( const eObjectType& _eObjType, const eArmorUpgradeType& _eUpgradeType );

};

