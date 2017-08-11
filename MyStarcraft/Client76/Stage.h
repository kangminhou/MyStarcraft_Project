#pragma once
#include "scene.h"
#include "WeaponMgr.h"

class CStage :
	public CScene
{
private:
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
