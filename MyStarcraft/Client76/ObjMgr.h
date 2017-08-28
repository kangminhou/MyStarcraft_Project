#pragma once

#include "Include.h"

class CGameObject;
class CGameEntity;
class CBackground;
class CObjMgr
{
	DECLARE_SINGLETON( CObjMgr );

private:
	enum { EntitySpaceArrSize = 20 };

private:
	list<CGameObject*>	m_ObjList[OBJ_TYPE_MAX];
#ifdef _DIV_SPACE_LIST
	list<CGameEntity*>	m_EntitySpaceList[OBJ_TYPE_MAX][TOTAL_SPACE_NUM];
#else
	CGameEntity*		m_EntitySpaceArr[OBJ_TYPE_MAX][TOTAL_SPACE_NUM][EntitySpaceArrSize];
#endif
	CBackground*	m_pBackground;

public:
	void AddGameObject( CGameObject* pObject, eObjectType eType );
	void InsertEntitySpaceData( CGameEntity* _pGameEntity );
	void EraseEntitySpaceData( CGameEntity* _pGameEntity, const int& iSpaceKey );
	bool CheckEntitysCol( vector<CGameEntity*>* _pOut, const CGameEntity* _pGameEntity, int iVecLimitSize = -1 );
	bool CheckEntitysCol( vector<CGameEntity*>* _pOut, const CGameEntity* _pGameEntity, eObjectType _eCheckID, int iVecLimitSize = -1 );
	bool CheckNearEntitys( vector<CGameEntity*>* _pOut, const CGameEntity* _pGameEntity, eObjectType _eCheckID, int iVecLimitSize = -1 );
	bool CheckNearEntitys( vector<CGameEntity*>* _pOut, const D3DXVECTOR3 _vStartPos, const float& _fCheckRadius, const eObjectType& _eCheckID, int iVecLimitSize = -1 );
	bool CheckNearEntitys( vector<CGameEntity*>* _pOut, const CGameEntity* _pGameEntity, const float& _fCheckRadius, const eObjectType& _eCheckID, int iVecLimitSize = -1 );
	bool CheckDragEntitys( vector<CGameEntity*>& _vecOut, const MOUSE_DRAG_DATA& _tDragData, const eObjectType& eID );
	bool GetSameEntitys( vector<CGameEntity*>& _vecOut, const CGameEntity* _pGameEntity, const float& _fRadius, const eObjectType& _eID, int iVecLimitSize = -1 );

public:
	list<CGameObject*>* GetList( eObjectType eType );

	template<typename T>
	T* FindGameObject( int iIndex = 0 );

public:
	HRESULT	Initialize( void );
	int		Update( void );
	void	Render( void );
	void	Release( void );

	void	Release( eObjectType eType );

private:
	int GetSpaceIndex( const D3DXVECTOR3& _vPos );

private:
	CObjMgr( void );
public:
	~CObjMgr( void );
};

template<typename T>
inline T * CObjMgr::FindGameObject( int iIndex )
{
	for ( size_t i = 0; i < OBJ_TYPE_MAX; ++i )
	{
		list<CGameObject*>::iterator iter = m_ObjList[i].begin();
		list<CGameObject*>::iterator iter_end = m_ObjList[i].end();

		for ( iter; iter != iter_end; ++iter )
		{
			if ( typeid(T) == typeid(*(*iter)) )
			{
				if ( iIndex == 0 )
					return dynamic_cast<T*>((*iter));
				else
					--iIndex;
			}
		}
	}
	return NULL;
}
