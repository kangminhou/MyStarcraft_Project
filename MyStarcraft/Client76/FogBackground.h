#pragma once
#include "GameObject.h"

class CFogUnit;

class CFogBackground :
	public CGameObject
{
public:
	typedef struct tagTestTile
	{
		D3DXVECTOR3 vPos;
		D3DXMATRIX	matWorld;
		D3DCOLOR	color;

		int			iOption;

		bool		bAlreadyWatch;
		bool		bCheck;

	}TEST_TILE;

	enum { TEST_TILE_X = 100, TEST_TILE_Y = 100, TEST_TILE_CX = 32, TEST_TILE_CY = 32 };

private:
	const TEX_INFO*		m_pTexture;
	const TEX_INFO*		m_pTexture2;
	vector<TEST_TILE*>	m_vecTIle;
	vector<TEST_TILE*>	m_vecTIle2;

	vector<int>			m_vecSeeIndex;
	vector<int>			m_vecSawIndex;
	vector<int>			m_vecEtcIndex;

	vector<CFogUnit*>	m_vecUnits;

public:
	CFogBackground();
	virtual ~CFogBackground();

public:
	virtual HRESULT Initialize( void ) override;
	virtual int Update( void ) override;
	virtual void Render( void ) override;
	virtual void Release( void ) override;

public:
	virtual void UpdatePosition( const D3DXVECTOR3& vPrevPos ) override;

	int GetTileIndex( const D3DXVECTOR3& _vPos );

	bool CheckRay( const D3DXVECTOR3& _vStart, const D3DXVECTOR3& _vEnd );
	bool CheckRay( const int& _iStartIndex, const int& _iEndIndex );

	void Scroll();

};

