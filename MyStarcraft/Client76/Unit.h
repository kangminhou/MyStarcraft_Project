#pragma once
#include "GameEntity.h"

class CEntityPattern;
class CWeaponMgr;

class CUnit :
	public CGameEntity
{
protected:
	static CWeaponMgr*		m_pWeaponMgr;

	/* 유닛 방어력 업글 타입 변수가 올 자리.. */

public:
	CUnit();
	virtual ~CUnit();

public:
	static void SetWeaponManager( CWeaponMgr* _pWeaponMgr );

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT	Initialize( void ) override;
	virtual int Update( void ) override;
	virtual void Release( void ) override;

public:
	virtual void InitAnimation() PURE;
	virtual void InitPattern() PURE;

};

