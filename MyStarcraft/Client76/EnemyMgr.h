#pragma once
#include "Include.h"
#include "EntityMgr.h"

class CGameEntity;

class CEnemyMgr
{
	DECLARE_SINGLETON( CEnemyMgr )

private:
	list<CGameEntity*>*	m_pAllEntityList[CEntityMgr::Entity_UnitEnd];

	CEntityMgr*			m_pEntityMgr;

	float				m_fCurTime;
	float				m_fStartTime;

	int					m_iLevel;
	
public:
	CEnemyMgr();
	~CEnemyMgr();

	void Initialize();
	void Update();
	void Release();

};

