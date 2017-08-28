#pragma once
#include "scene.h"
#include "WeaponMgr.h"

class CAStarManager;
class CEnemyMgr;
class CObjMgr;
class CUIMgr;

class CStage :
	public CScene
{
private:
	CAStarManager*	m_pAStarMgr;
	CEnemyMgr*		m_pEnemyMgr;
	CObjMgr*		m_pObjMgr;
	CUIMgr*			m_pUIMgr;

	CWeaponMgr m_weaponManager;
	int nCnt;

public:
	virtual HRESULT	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(void);
	virtual void	Release(void);

public:
	CStage(void);
	virtual ~CStage(void);
};
