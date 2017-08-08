#pragma once
#include "GameEntity.h"

class CWeapon;
class CEntityPattern;
class CWeaponMgr;

class CUnit :
	public CGameEntity
{
protected:
	CWeapon*				m_pGroundAttWeapon;			// ���� ������ ������ ����..
	CWeapon*				m_pAirAttWeapon;			// ���� ������ ������ ����..

	UNIT_GENERATE_DATA		m_tGenerateData;			// ���� ���� �� �ʿ��� ������..

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
	virtual void Render( void ) override;
	virtual void Release( void ) override;

public:
	// CGameEntity��(��) ���� ��ӵ�
	virtual void SetPattern( const eGameEntityPattern& _ePatternKind ) PURE;

public:
	virtual void InitAnimation() PURE;
	virtual void InitPattern() PURE;

};

