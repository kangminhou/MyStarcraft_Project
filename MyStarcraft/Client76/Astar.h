#pragma once
#include "Include.h"

class CBackground;
class CGameEntity;

class CAStar
{
public:
	enum ePathFindEventType
	{
		Event_Success_PathFind,
		Event_Fail_PathFind,
		Event_Index_OutRange_TileSize,
		Event_Fail_Find_NearIndex,
	};

private:
	list<NODE*>		m_OpenList;		//������ ���
	list<NODE*>		m_CloseList;	//������ ���
	CBackground*	m_pBackground;

	CGameEntity*	m_pCheckEntity;

private:
	int		m_iStartIndex;
	int		m_iEndIndex;

public:
	void SetBackground( CBackground* _pBackground );
	void SetEntity( CGameEntity* _pEntity );

public:
	int AStarStartPos(const D3DXVECTOR3& vStartPos, const D3DXVECTOR3& vEndPos, vector<D3DXVECTOR3>& vecGetData, const bool& _bCheckEntityTile = TRUE);

private:
	int AStarStart( vector<D3DXVECTOR3>& vecGetData, 
					 const bool& _bCheckEntityTile );

	int MakeRoute(vector<D3DXVECTOR3>& vecGetData, const bool& _bCheckEntityTile);
	NODE* MakeNode(int iIndex, NODE* pParent, const vector<TILE*>* pTile);

	bool ListCheck(const int& iIndex);

	void Release(void);

public:
	int  GetTileIndex(const D3DXVECTOR3& vPos);

public:
	//[17.08.03_00] : ���˻縦 ���� �Լ�.
	static bool Compare(const NODE* pNode1, const NODE* pNode2);

public:
	CAStar(void);
	~CAStar(void);
};
