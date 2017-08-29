#pragma once
#include "GameObject.h"
#include "GameEntity.h"

class CBackground :
	public CGameObject
{
public:
	enum { ENTITY_TILE_DIV_X = 2, ENTITY_TILE_DIV_Y = 2, ENTITY_TOTAL_TILE_DIV = ENTITY_TILE_DIV_X * ENTITY_TILE_DIV_Y };

	typedef struct tagEntityTileData
	{
		BYTE						byTileOption;
		const CGameEntity*			pEntityArr[ENTITY_TOTAL_TILE_DIV];

		tagEntityTileData()
			: byTileOption(0)
		{
			for ( int i = 0; i < ENTITY_TOTAL_TILE_DIV; ++i )
				pEntityArr[i] = NULL;
		}

	}ENTITY_TILE_DATA;

	typedef struct tagFogData
	{
		bool		bNotFog;
		bool		bAlreadySeeTile;

		tagFogData()
			: bNotFog( false ), bAlreadySeeTile( false )
		{}

	}FOG_DATA;

private:
	LPD3DXFONT			m_pFont;

	vector<TILE*>		m_vecTile;
	ENTITY_TILE_DATA	m_entityTileData[TILEX * TILEY * ENTITY_TOTAL_TILE_DIV];

	list<CGameObject*>*	m_pPlayerObjList;

	const TEX_INFO*		m_pBackgroundTexture;
	const TEX_INFO*		m_pBlackTileTexture;
	vector<const TEX_INFO*>	m_vecTileTexture;

	vector<FOG_DATA*>	m_vecFogData;
	vector<FOG_DATA*>	m_vecNotFog;

	bool				m_bActiveFog;
	bool				m_bActiveTile;

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

	void	EraseUnitData( const vector<_Dlong>& _vecKey );
	void	UpdateUnitData( const CGameEntity* _pEntity, vector<_Dlong>& _vecKey );

	bool	CheckCanGoTile( const int& _iIndex, const BYTE& _byDir, CGameEntity* _pEntity, const bool& _bCheckEntityData );
	BYTE	CalcEntityTileData( const int& iIndex, CGameEntity* _pEntity );

	int		CalcNearCanGoTile( const int& _iStartIndex, const int& _iEndIndex, const bool& _bCheckEntityData );
	bool	CheckConnectPath( CGameEntity* _pEntity, const int& _iStartIndex, const int& _iEndIndex, const bool& _bCheckEntityData );

	void	ObjectDataUpdate( CGameEntity* pEntity, int iTileOption = 1 );

public:
	HRESULT Initialize(void);
	int     Update(void);
	void	Render(void);
	void	Release(void);

public:
	void MiniMapRender(void);

private:
	void LoadTileData();

	bool CheckRay( const D3DXVECTOR3& _vStart, const D3DXVECTOR3& _vEnd );
	bool CheckRay( const int& _iStartIndex, const int& _iEndIndex );

};

