#pragma once
#include "Include.h"

class CAStar;
class CMove;

/* AStar 를 관리해줄 매니저.. */
class CAStarManager
{
	DECLARE_SINGLETON(CAStarManager)
private:
	typedef list<pair<CMove*, CAStar*>>				PATH_FIND_LIST;
	typedef list<pair<CMove*, CAStar*>>::iterator	PATH_FIND_LIST_ITERATOR;

	enum { MaxUpdateNum = 15 };

private:
	PATH_FIND_LIST			m_PathfindList;
	PATH_FIND_LIST_ITERATOR	m_CurList_Iter;

public:
	CAStarManager();
	~CAStarManager();

public:
	int AddAStar( CMove* _pMoveComponent, CAStar* _pAStarComponent, const D3DXVECTOR3 & _vStartPos, 
				   const D3DXVECTOR3 & _vEndPos, vector<D3DXVECTOR3>& vecGetData, const bool & _bCheckEntityTile);

	void EraseAStar( CMove* _pMoveComponent );

public:
	void Update();
	void Release();

};

