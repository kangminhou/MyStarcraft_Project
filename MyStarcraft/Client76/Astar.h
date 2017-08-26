#pragma once
#include "Include.h"

class CBackground;
class CGameEntity;

class CAStar
{
public:
	enum ePathFindEventType
	{
		Event_Can_PathFind,
		Event_Success_PathFind,
		Event_Fail_PathFind,
		Event_Index_OutRange_TileSize,
		Event_Fail_Find_NearIndex,
		Event_Not_Ended_PathFind,
	};

	enum { Limit_Close_List_Num = 2000 };

private:
	list<NODE*>		m_OpenList;		//조사할 대상
	list<NODE*>		m_CloseList;	//조사한 대상
	CBackground*	m_pBackground;

	CGameEntity*	m_pCheckEntity;
	const vector<TILE*>*	m_pVecTile;

	vector<D3DXVECTOR3>*	m_pVecBestPath;

	NODE*			m_pCurCheckNode;

	D3DXVECTOR3		m_vDestination;

	bool			m_bCheckEntityTile;

private:
	int		m_iStartIndex;
	int		m_iEndIndex;

public:
	void SetBackground( CBackground* _pBackground );
	void SetEntity( CGameEntity* _pEntity );

public:
	D3DXVECTOR3 GetDestination() const;

public:
	int AStarSetting( const D3DXVECTOR3& _vStartPos, const D3DXVECTOR3& _vEndPos, vector<D3DXVECTOR3>& vecGetData, const bool& _bCheckEntityTile = TRUE );

	int PathFind();

private:
	int AStarSetting( const D3DXVECTOR3& _vStartPos );

	int SettingPathFind();
	int MakeRoute(vector<D3DXVECTOR3>& vecGetData, const bool& _bCheckEntityTile);
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
