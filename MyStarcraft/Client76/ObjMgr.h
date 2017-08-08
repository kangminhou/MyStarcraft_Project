#pragma once

#include "Include.h"

class CGameObject;
class CObjMgr
{
	DECLARE_SINGLETON(CObjMgr);

private:
	list<CGameObject*>	m_ObjList[OBJ_TYPE_MAX];

public:
	void AddGameObject(CGameObject* pObject, eObjectType eType);

public:
	list<CGameObject*>* GetList(eObjectType eType);

public:
	HRESULT	Initialize(void);
	int		Update(void);
	void	Render(void);
	void	Release(void);

	void	Release(eObjectType eType);


private:
	CObjMgr(void);
public:
	~CObjMgr(void);
};
