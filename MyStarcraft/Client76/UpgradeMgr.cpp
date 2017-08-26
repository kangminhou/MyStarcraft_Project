#include "stdafx.h"
#include "UpgradeMgr.h"

#include "WeaponMgr.h"

#include "GameEntity.h"

IMPLEMENT_SINGLETON(CUpgradeMgr)


CUpgradeMgr::CUpgradeMgr()
{
}


CUpgradeMgr::~CUpgradeMgr()
{
}

void CUpgradeMgr::SetWeaponMgr( CWeaponMgr * _pWeaponMgr )
{
	this->m_pWeaponMgr = _pWeaponMgr;
}

void CUpgradeMgr::Initialize()
{
	for ( int i = 0; i < 2; ++i )
	{
		for ( int j = 0; j < Upgrade_Armor_End; ++j )
		{
			m_ArmorUpgradeDataArr[i][j].second = 0;
		}
	}
}

void CUpgradeMgr::AddArmorUpgradeObserver( CGameEntity* _pObserver, const eArmorUpgradeType & _eUpgradeType )
{
	if ( _eUpgradeType < 0 || _eUpgradeType >= Upgrade_Armor_End )
		return;

	eObjectType objType = _pObserver->GetObjectType();

	if ( objType < OBJ_TYPE_USER || objType > OBJ_TYPE_USER2 )
		return;

	int iIndex = _pObserver->GetObjectType() - OBJ_TYPE_USER;

	auto& observerList = this->m_ArmorUpgradeDataArr[iIndex][_eUpgradeType].first;
	
	observerList.push_back( _pObserver );

	_pObserver->UpgradeArmor( this->m_ArmorUpgradeDataArr[iIndex][_eUpgradeType].second );
}

void CUpgradeMgr::EraseArmorUpgradeObserver( CGameEntity * _pObserver, const eArmorUpgradeType & _eUpgradeType )
{
	if ( _eUpgradeType < 0 || _eUpgradeType >= Upgrade_Armor_End )
		return;

	eObjectType objType = _pObserver->GetObjectType();

	if ( objType < OBJ_TYPE_USER || objType > OBJ_TYPE_USER2 )
		return;

	int iIndex = _pObserver->GetObjectType() - OBJ_TYPE_USER;

	auto& observerList = this->m_ArmorUpgradeDataArr[iIndex][_eUpgradeType].first;

	for ( auto iter = observerList.begin(); iter != observerList.end(); ++iter )
	{
		if ( (*iter) == _pObserver )
		{
			observerList.erase( iter );
			break;
		}
	}
}

void CUpgradeMgr::UpgradeWeapon( const eObjectType& _eObjType, const eWeaponUpgradeType& _eUpgradeType )
{
	if ( _eUpgradeType < 0 || _eUpgradeType >= Upgrade_Weapon_End )
		return;

	if ( _eObjType < OBJ_TYPE_USER || _eObjType > OBJ_TYPE_USER2 )
		return;

	m_pWeaponMgr->UpgradeWeapon( _eObjType, _eUpgradeType );
}

void CUpgradeMgr::UpgradeArmor( const eObjectType& _eObjType, const eArmorUpgradeType & _eUpgradeType )
{
	if ( _eUpgradeType < 0 || _eUpgradeType >= Upgrade_Armor_End )
		return;

	if ( _eObjType < OBJ_TYPE_USER || _eObjType > OBJ_TYPE_USER2 )
		return;

	int iIndex = _eObjType - OBJ_TYPE_USER;

	auto& observerList = this->m_ArmorUpgradeDataArr[iIndex][_eUpgradeType].first;
	BYTE& byUpgradeNum = this->m_ArmorUpgradeDataArr[iIndex][_eUpgradeType].second;

	++byUpgradeNum;

	for ( auto& iter : observerList )
	{
		iter->UpgradeArmor( byUpgradeNum );
	}
}
