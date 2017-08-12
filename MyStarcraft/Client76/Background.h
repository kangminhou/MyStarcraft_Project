#pragma once
#include "GameObject.h"

class CGameEntity;

class CBackground :
	public CGameObject
{
public:
	typedef struct tagEntityTileData
	{
		BYTE						byTileOption;
		list<const CGameEntity*>	entityList;

		tagEntityTileData()
			: byTileOption(0)
		{}

	}ENTITY_TILE_DATA;

	enum { ENTITY_TILE_DIV_X = 2, ENTITY_TILE_DIV_Y = 2, ENTITY_TOTAL_TILE_DIV = ENTITY_TILE_DIV_X * ENTITY_TILE_DIV_Y };

private:
	vector<TILE*>		m_vecTile;
	ENTITY_TILE_DATA	m_entityTileData[TILEX * TILEY * ENTITY_TOTAL_TILE_DIV];

	const TEX_INFO*		m_pBackgroundTexture;
	vector<const TEX_INFO*>	m_vecTileTexture;

public:
	vector<TILE*>*	GetTile(void)
	{
		return &m_vecTile;
	}

	BYTE GetUnitTileData( int iIndex )
	{
		return m_entityTileData[iIndex].byTileOption;
	}

public:
	CBackground();
	virtual ~CBackground();

public:
	void	SetScroll( int iScrollX, int iScrollY );
	int		GetTileIndex(const D3DXVECTOR3& vMousePos);
	void	TileChange(const D3DXVECTOR3& vMousePos, const int& iOption, const int& iHeight);

	bool Picking(const D3DXVECTOR3& vPos, const int& iIndex);

	void	EraseUnitData( const CGameEntity* _pEntity, const D3DXVECTOR3& _vPos );
	void	UpdateUnitData( const CGameEntity* _pEntity );

	bool	CheckCanGoTile( const int& iIndex, const BYTE& byDir, CGameEntity* _pEntity );
	BYTE	CalcEntityTileData( const int& iIndex, CGameEntity* _pEntity );

public:
	HRESULT Initialize(void);
	int     Update(void);
	void	Render(void);
	void	Release(void);

public:
	void MiniMapRender(void);

private:
	void LoadTileData();

};

