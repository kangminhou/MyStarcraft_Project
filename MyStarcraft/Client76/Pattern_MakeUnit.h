#pragma once
#include "EntityPattern.h"
#include "GameEntity.h"

class CBuilding;
class CPlayer;
class CUIMgr;

class CPattern_MakeUnit :
	public CEntityPattern
{
private:
	CBuilding*		m_pBuilding;
	CGameEntity::ORDER_DATA	m_tCurOrderData;
	CPlayer*		m_pPlayer;
	CUIMgr*			m_pUIMgr;

	float			m_fRestTime;
	float			m_fMaxTime;

	bool			m_bStop;

public:
	CPattern_MakeUnit();
	virtual ~CPattern_MakeUnit();

public:
	// CEntityPattern을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void OnEnable() override;
	virtual int Update() override;
	virtual void Release() override;

private:
	bool Setting();

};

