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

	template<typename T>
	T* FindGameObject( int iIndex = 0 );

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

template<typename T>
inline T * CObjMgr::FindGameObject( int iIndex )
{
	for(size_t i = 0; i < OBJ_TYPE_MAX; ++i)
	{
		list<CGameObject*>::iterator iter = m_ObjList[i].begin();
		list<CGameObject*>::iterator iter_end = m_ObjList[i].end();

		for(iter; iter != iter_end; ++iter)
		{
			if( typeid(T) == typeid( *(*iter) ) )
			{
				if(iIndex == 0)
					return dynamic_cast<T*>((*iter));
				else
					--iIndex;
			}
		}
	}
	return NULL;
}
