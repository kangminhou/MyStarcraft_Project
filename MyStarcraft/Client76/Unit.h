#pragma once
#include "GameEntity.h"

class CWeapon;
class CUnitPattern;

class CUnit :
	public CGameEntity
{
private:
	CWeapon*				m_pGroundAttWeapon;			// ���� ������ ������ ����..
	CWeapon*				m_pAirAttWeapon;			// ���� ������ ������ ����..

	UNIT_GENERATE_DATA		m_tGenerateData;			// ���� ���� �� �ʿ��� ������..

	map<wstring, CUnitPattern*>	m_mapPatterns;
	CUnitPattern*			m_pCurActPattern;

	BYTE					m_byDirAnimIndex;

public:
	CUnit();
	virtual ~CUnit();

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT	Initialize( void ) override;
	virtual int Update( void ) override;
	virtual void Render( void ) override;
	virtual void Release( void ) override;

public:
	void DecideDirAnimIndex();
	void UpdateDir();

};

