#pragma once

#include "Include.h"

class CTool76View;
class CBackGround
{
private:
	vector<TILE*>		m_vecTile;

	CTool76View*		m_pMainView;			//뷰창에 있는 스크롤 정보를 얻어오기 위해서.

public:
	void SetMainView(CTool76View* pView)
	{
		m_pMainView = pView; 
	}

	vector<TILE*>*	GetTile(void)
	{
		return &m_vecTile;
	}

public:
	CTool76View* GetMainView( void )
	{
		return m_pMainView;
	}

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

public:
	CBackGround(void);
	~CBackGround(void);
};
