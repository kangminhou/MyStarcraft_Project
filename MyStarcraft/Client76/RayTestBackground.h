#pragma once
#include "GameObject.h"

class CRayTestBackground :
	public CGameObject
{
private:
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

	//Ÿ�� ������ ������ ����ü.
	typedef struct tagTile
	{
		D3DXVECTOR3 vPos;
		D3DXVECTOR3 vSize;

		BYTE		byOption;		// �浹 ����(�����ִ�, ����, ��Ż:����,����.., �̺�Ʈ)..
		BYTE		byDrawID;		// Ÿ�� �̹���..
		BYTE		byHeight;		// Ÿ���� ����..

		tagTile()
			: byHeight(0), byDrawID(0), byOption(0)
		{}

	}TEST_TILE;

private:
	vector<TEST_TILE*> m_vecTile;
	vector<const TEX_INFO*> m_vecTileTexture;

public:
	CRayTestBackground();
	virtual ~CRayTestBackground();

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Initialize( void ) override;
	virtual int Update( void ) override;
	virtual void Render( void ) override;
	virtual void Release( void ) override;
};

