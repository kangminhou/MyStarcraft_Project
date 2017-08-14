#pragma once
#include "Include.h"

class CBackground;
class CGameEntity;

class CAStar
{
private:
	list<NODE*>		m_OpenList;		//조사할 대상
	list<NODE*>		m_CloseList;	//조사한 대상
	CBackground*	m_pBackground;

	CGameEntity*	m_pCheckEntity;

private:
	int		m_iStartIndex;
	int		m_iEndIndex;

public:
	void SetBackground( CBackground* _pBackground );
	void SetEntity( CGameEntity* _pEntity );

public:
	bool AStarStartPos(const D3DXVECTOR3& vStartPos, const D3DXVECTOR3& vEndPos, vector<D3DXVECTOR3>& vecGetData);

private:
	bool AStarStart( const int& iStartIndex,
					 const int& iEndIndex,
					 vector<D3DXVECTOR3>& vecGetData );

	bool MakeRoute(vector<D3DXVECTOR3>& vecGetData);
	NODE* MakeNode(int iIndex, NODE* pParent, const vector<TILE*>* pTile);

	bool ListCheck(const int& iIndex);

	void Release(void);

public:
	int  GetTileIndex(const D3DXVECTOR3& vPos);

public:
	//[17.08.03_00] : 비용검사를 위한 함수.
	static bool Compare(const NODE* pNode1, const NODE* pNode2);

public:
	CAStar(void);
	~CAStar(void);
};
