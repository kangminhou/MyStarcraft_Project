#pragma once
#include "GameEntity.h"

class CPlayer;

class CBuilding :
	public CGameEntity
{
protected:
	/* 
	 * 건물은 건설 시의 애니메이션을 가지고 있음..
	 * 
	 */
	static const TEX_INFO*	m_pBasicBuildTextureArr[3][3];	// 건물을 지을 때 밑바탕 애니메이션..

	D3DXVECTOR3				m_vTileOccupy;					// 차지하는 타일 개수..
	map<wstring, vector<const TEX_INFO*>>	m_mapAllTexture;
	const TEX_INFO*			m_pCurDrawTexture;

	CPlayer*				m_pPlayer;

	bool					m_bSuccessBuild;				// 다 지어진 건물인가??..
	bool					m_bApplyCol;

public:
	CBuilding();
	virtual ~CBuilding();

public:
	void SetApplyCol( const bool& _bApplyCol );

public:
	bool GetIsSuccessBuild() const;

public:
	virtual HRESULT Initialize( void );
	virtual int Update( void );

	void FrameRender( const D3DXMATRIX& _matWorld );

public:
	virtual void UpdateLookAnimIndex() override;
	virtual void SuccessBuild();

protected:
	virtual void DecideTexture() PURE;
	virtual void InitTexture() PURE;

protected:
	D3DXVECTOR3 CalcNearEmptySpace();

public:
	static void InitBasicBuildTexture();

};

