#include "StdAfx.h"
#include "ObjMgr.h"
#include "GameObject.h"

IMPLEMENT_SINGLETON(CObjMgr);

void CObjMgr::AddGameObject(CGameObject* pObject, eObjectType eType)
{
	m_ObjList[eType].push_back(pObject);
}

list<CGameObject*>* CObjMgr::GetList( eObjectType eType )
{
	return &m_ObjList[eType];
}

HRESULT CObjMgr::Initialize(void)
{
	return S_OK;
}

int CObjMgr::Update(void)
{
	for(size_t i = 0; i < OBJ_TYPE_MAX; ++i)
	{
		list<CGameObject*>::iterator iter = m_ObjList[i].begin();
		list<CGameObject*>::iterator iter_end = m_ObjList[i].end();

		for(iter; iter != iter_end; )
		{
			if( (*iter)->Update() == 1 )
			{
				safe_delete(*iter);
				iter = m_ObjList[i].erase(iter);
			}
			else
				++iter;
		}
	}

	return 0;
}

void CObjMgr::Render(void)
{
	for(size_t i = 0; i < OBJ_TYPE_MAX; ++i)
	{
		list<CGameObject*>::iterator iter = m_ObjList[i].begin();
		list<CGameObject*>::iterator iter_end = m_ObjList[i].end();

		for(iter; iter != iter_end; ++iter)
		{
			(*iter)->Render();
		}
	}
}

void CObjMgr::Release(void)
{
	for(size_t i = 0; i < OBJ_TYPE_MAX; ++i)
	{
		list<CGameObject*>::iterator iter = m_ObjList[i].begin();
		list<CGameObject*>::iterator iter_end = m_ObjList[i].end();

		for(iter; iter != iter_end; ++iter)
		{
			safe_delete(*iter);
		}
		m_ObjList[i].clear();
	}
}

void CObjMgr::Release(eObjectType eType)
{
	for(size_t i = 0; i < OBJ_TYPE_MAX; ++i)
	{
		list<CGameObject*>::iterator iter = m_ObjList[i].begin();
		list<CGameObject*>::iterator iter_end = m_ObjList[i].end();

		if(eType == i)
			continue;

		for(iter; iter != iter_end; ++iter)
		{
			safe_delete(*iter);
		}
		m_ObjList[i].clear();
	}
}

CObjMgr::CObjMgr(void)
{
}

CObjMgr::~CObjMgr(void)
{
	Release();
}
