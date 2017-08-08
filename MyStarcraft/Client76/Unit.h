#pragma once
#include "GameEntity.h"

class CWeapon;
class CUnitPattern;

class CUnit :
	public CGameEntity
{
private:
	CWeapon*				m_pGroundAttWeapon;			// 지상 유닛을 공격할 무기..
	CWeapon*				m_pAirAttWeapon;			// 공중 유닛을 공격할 무기..

	UNIT_GENERATE_DATA		m_tGenerateData;			// 유닛 생성 시 필요한 데이터..

	map<wstring, CUnitPattern*>	m_mapPatterns;
	CUnitPattern*			m_pCurActPattern;

	BYTE					m_byDirAnimIndex;

public:
	CUnit();
	virtual ~CUnit();

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT	Initialize( void ) override;
	virtual int Update( void ) override;
	virtual void Render( void ) override;
	virtual void Release( void ) override;

public:
	void DecideDirAnimIndex();
	void UpdateDir();

};

