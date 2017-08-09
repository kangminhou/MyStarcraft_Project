#pragma once
#include "GameObject.h"
class CBackground :
	public CGameObject
{
private:
	vector<TILE*>		m_vecTile;

	const TEX_INFO*		m_pBackgroundTexture;

public:
	vector<TILE*>*	GetTile(void)
	{
		return &m_vecTile;
	}

public:
	CBackground();
	virtual ~CBackground();

public:
	void	SetScroll( int iScrollX, int iScrollY );
	int		GetTileIndex(const D3DXVECTOR3& vMousePos);
	void	TileChange(const D3DXVECTOR3& vMousePos, const int& iOption, const int& iHeight);

	bool Picking(const D3DXVECTOR3& vPos, const int& iIndex);

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

