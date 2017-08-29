#pragma once
#include "GameEntity.h"

class CPlayer;
class CResearchMgr;

class CBuilding :
	public CGameEntity
{
protected:
	/* 
	 * �ǹ��� �Ǽ� ���� �ִϸ��̼��� ������ ����..
	 * 
	 */
	CResearchMgr*			m_pResearchMgr;

	eResearchKind			m_eResearchKind;

	static const TEX_INFO*	m_pBasicBuildTextureArr[3][3];	// �ǹ��� ���� �� �ع��� �ִϸ��̼�..

	D3DXVECTOR3				m_vTileOccupy;					// �����ϴ� Ÿ�� ����..
	map<wstring, vector<const TEX_INFO*>>	m_mapAllTexture;
	const TEX_INFO*			m_pCurDrawTexture;

	bool					m_bSuccessBuild;				// �� ������ �ǹ��ΰ�??..
	bool					m_bApplyCol;
	bool					m_bCanBuild;
	bool					m_bClick;
	bool					m_bUseActiveTexture;

	const TEX_INFO*			m_pBuildRectTexture[2];
	vector<const TEX_INFO*>	m_vecResearchShowTexture;

	vector<TILE*>*			m_pVecTile;

public:
	CBuilding();
	virtual ~CBuilding();

public:
	void SetApplyCol( const bool& _bApplyCol );
	void SetClick( const bool& _bClick );

public:
	bool GetIsSuccessBuild() const;
	bool GetIsCanBuild() const;
	eResearchKind GetResearchKind();

public:
	virtual HRESULT Initialize( void );
	virtual int Update( void );
	virtual void Render( void );

	void FrameRender( const D3DXMATRIX& _matWorld );
	void RectRender( const RECT& _rcDraw );
	void BuildStart();

	virtual void SuccessOrder( const CGameEntity::eGameEntityPattern& _ePatternKind ) override;
	void SuccessResearch();

	void UpdateOrderRestTime( const BYTE& _byRatio );

public:
	virtual void UpdateLookAnimIndex() override;
	virtual void SuccessBuild();

protected:
	virtual void DecideTexture() PURE;
	virtual void InitTexture() PURE;
	virtual bool CheckCanBuild( const int& _iIndex );

protected:
	D3DXVECTOR3 CalcNearEmptySpace();

	void ShowUpdateOrderData();

public:
	static void InitBasicBuildTexture();

};

