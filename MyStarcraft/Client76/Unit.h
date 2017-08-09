#pragma once
#include "GameEntity.h"

class CWeapon;
class CEntityPattern;
class CWeaponMgr;

class CUnit :
	public CGameEntity
{
protected:
	CWeapon*				m_pGroundAttWeapon;			// 지상 유닛을 공격할 무기..
	CWeapon*				m_pAirAttWeapon;			// 공중 유닛을 공격할 무기..

	UNIT_GENERATE_DATA		m_tGenerateData;			// 유닛 생성 시 필요한 데이터..

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
	// CGameEntity을(를) 통해 상속됨
	virtual void SetPattern( const eGameEntityPattern& _ePatternKind ) PURE;

public:
	virtual void InitAnimation() PURE;
	virtual void InitPattern() PURE;

};

