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

private:
	vector<TILE*>		m_vecTile;
	ENTITY_TILE_DATA	m_entityTileData[TILEX * TILEY];

	const TEX_INFO*		m_pBackgroundTexture;

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

	void	UpdateUnitPosData( const CGameEntity* _pEntity, bool bErase = false );

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

