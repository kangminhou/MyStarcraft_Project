#pragma once
#include "Include.h"
#include "EntityMgr.h"

class CObjMgr;
class CGameEntity;

class CEnemyMgr
{
	DECLARE_SINGLETON( CEnemyMgr )

private:
	typedef struct tagEnemyRushLevelData
	{
		vector<CGameEntity*> vecEntity;
		bool bRush;

		tagEnemyRushLevelData()
			: bRush(false)
		{}

	}ENEMY_RUSH_LEVEL_DATA;

private:
	list<CGameEntity*>*	m_pAllEntityList[CEntityMgr::Entity_UnitEnd];

	CEntityMgr*			m_pEntityMgr;
	CObjMgr*			m_pObjMgr;

	float				m_fCurTime;
	float				m_fStartTime;

	int					m_iLevel;

	vector<ENEMY_RUSH_LEVEL_DATA>	m_vecRushData;
	
public:
	CEnemyMgr();
	~CEnemyMgr();

	void Initialize();
	void Update();
	void Release();

	void AddEntityToLevel( const int& _iLevel, CGameEntity* _pEntity );

};

