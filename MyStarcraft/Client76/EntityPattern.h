#pragma once
#include "Component.h"

/* �ǹ� or ������ ����� ������ �θ� Ŭ����.. */
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
	// CComponent��(��) ���� ��ӵ�
	virtual void Initialize() PURE;
	virtual void OnEnable() PURE;
	virtual int Update() PURE;
	virtual void Release() PURE;

};

