#pragma once

#include "Include.h"
#include "GameObject.h"

class CTestAStar :
	public CGameObject
{
	struct CompareNode
	{
		bool operator()(NODE* pNode1, NODE* pNode2){
			return pNode1->fCost < pNode2->fCost;
		}
	};

	enum eTileState
	{
		TILE_IDLE,
		TILE_ENDINDEX,
		TILE_CHECK,
		TILE_PATH,
		TILE_STARTINDEX,
		TILE_WALL,
		TILE_WAYPOINT
	};

	//타일 정보를 보관할 구조체.
	typedef struct tagTile
	{
		D3DXVECTOR3 vPos;
		D3DXVECTOR3 vSize;

		BYTE		byOption;		// 충돌 여부(갈수있다, 없다, 포탈:마을,상점.., 이벤트)..
		BYTE		byDrawID;		// 타일 이미지..
		BYTE		byHeight;		// 타일의 높이..

		tagTile()
			: byHeight(0), byDrawID(0), byOption(0)
		{}

	}TEST_TILE;

private:
	vector<TEST_TILE*>		m_vecTile;
	vector<const TEX_INFO*>	m_vecTileTexture;

	const TEX_INFO*		m_pBackgroundTexture;

	int	m_iStartIndex;
	int m_iEndIndex;

	int m_iClickIndex;
	bool m_bClickStartIndex;
	bool m_bMouseMoveCheck;

private:
	list<NODE*>		m_openList;
	list<int>		m_closeList;
	list<int>		m_bestList;

public:
	CTestAStar();
	~CTestAStar();

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize( void )override;
	virtual int Update( void ) override;
	virtual void Render( void );
	virtual void Release( void );

public:
	void AStarStart();

private:
	void ReleaseAStarData();
	void PathFind();
	NODE* MakeNode( const int& _iIndex, NODE* _pParent = NULL );
	bool ListCheck( const int& _iIndex );

};

