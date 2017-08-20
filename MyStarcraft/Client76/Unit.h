#pragma once
#include "GameEntity.h"

class CEntityPattern;
class CWeaponMgr;

class CUnit :
	public CGameEntity
{
protected:
	static CWeaponMgr*		m_pWeaponMgr;

	/* ���� ���� ���� Ÿ�� ������ �� �ڸ�.. */

public:
	CUnit();
	virtual ~CUnit();

public:
	static void SetWeaponManager( CWeaponMgr* _pWeaponMgr );

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT	Initialize( void ) override;
	virtual int Update( void ) override;
	virtual void Release( void ) override;

public:
	virtual void InitAnimation() PURE;
	virtual void InitPattern() PURE;

};

