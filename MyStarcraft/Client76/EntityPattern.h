#pragma once
#include "Component.h"

/* 건물 or 유닛이 사용할 패턴의 부모 클래스.. */
class CGameEntity;

class CEntityPattern :
	public CComponent
{
public:
	enum eReturnEvent
	{
		Event_Pattern_Change = -1,
		Event_None,
	};

protected:
	CGameEntity*	m_pGameEntity;

public:
	CEntityPattern();
	virtual ~CEntityPattern();

public:
	void SetGameEntity( CGameEntity* _pGameEntity );

public:
	// CComponent을(를) 통해 상속됨
	virtual void Initialize() PURE;
	virtual void OnEnable() PURE;
	virtual int Update() PURE;
	virtual void Release() PURE;

};

