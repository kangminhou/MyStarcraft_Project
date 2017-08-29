#pragma once
#include "Include.h"

class CUI;
class CGameEntity;
class CCorps;
class CMyButton;
class CPlayer;
class CMiniMap;
class CBuilding;
class CTextureMgr;
class CBackground;
class CMouse;
class CBuilding;

class CUIMgr
{
	DECLARE_SINGLETON(CUIMgr)

public:
	enum eShowFontType
	{
		Font_No_Mineral,
		Font_No_Gas,
		Font_No_Population,
		Font_Nuclear,
	};

private:
	CMouse*			m_pMouse;

	CTextureMgr*	m_pTextureMgr;
	CBackground*	m_pBackground;

	CUI*	m_pMainUI;

	CUI*	m_pFaceUI;
	CUI*	m_pCorpsWireFrameUIArr[12];
	CUI*	m_pEntityWireFrameUI;

	CUI*	m_pUnitUpgradeUI[3];
	CUI*	m_pUnitUpgradeFontUI[3];
	CUI*	m_pUnitUpgradeBackUI[3];
	RECT	m_rcUnitUpgradeCol[3];

	CUI*	m_pHpBarBackUI;
	CUI*	m_pHpBarUI;
	vector<HP_BAR_MATRIX_DATA> m_vecHpBarMatrixData;

	CUI*	m_pEntityHpUI;
	vector<pair<BYTE, D3DXMATRIX>>	m_vecHpShowData;

	const SELECT_UNIT_SHOW_DATA*	m_pShowUnitData;
	D3DXMATRIX		m_matDrawUnitName;
	D3DXMATRIX		m_matDrawUnitRank;

	CGameEntity*	m_pCurShowHpEntity;

	vector<CMyButton*>	m_vecButton;
	vector<CMyButton*>	m_vecDrawButton;

	vector<CUI*>	m_vecDeleteUI;

	CPlayer*		m_pPlayer;
	CCorps*			m_pCurShowUICorps;

	CMiniMap*		m_pMinimap;

	vector<CGameEntity*>	m_vecHpBarShowEntity;

	CBuilding*		m_pDrawBuilding;
	CBuilding*		m_pGetDataBuilding;

	CUI*			m_pShowResourceDataUI[3];
	vector<pair<BYTE, D3DXMATRIX>>	m_vecResourceShowFontData[3];

	CUI*			m_pBuildingOrderUI;
	CUI*			m_pOrderIconUI[5];
	CUI*			m_pRatioBackUI;

	const TEX_INFO*	m_pRatioTexture;
	D3DXMATRIX		m_ratioTextureWorldMatrix;

	bool			m_bShowBuildOrderUI;
	BYTE			m_byOrderRatio;
	size_t			m_iDrawOrderIconSize;

	LPD3DXSPRITE	m_pSprite;
	LPD3DXFONT		m_pFont;

	D3DXMATRIX		m_buildingWorldMatrix;
	RECT			m_rcBuilding;

	BYTE	m_byDrawUINum;
	BYTE	m_byDrawHpUINum;

	bool	m_bShowEntityUI;
	bool	m_bShowCorpsWire;
	bool	m_bDrawBuildingFrame;

	D3DXMATRIX		m_matFontWorld;
	wstring			m_wstrFont;
	float			m_fRestShowFontTime;
	bool			m_bShowFont;

public:
	CUIMgr();
	~CUIMgr();

public:
	CUI* GetHpBarBackUI() const;
	CUI* GetHpBarUI() const;
	CMiniMap* GetMiniMap() const;

public:
	void SetPlayer( CPlayer* _pPlayer );
	void SetOrderRatio( const BYTE& _byOrderRatio );
	void ShowFont( const eShowFontType& _eShowFontType );

	void ShowBuildingData( CBuilding* pBuilding );

public:
	void Initialize();
	void Update();
	void Render();
	void Release();
	
public:
	void ShowEntityUI( CCorps* _pCorps );
	void ShowButton( vector<BUTTON_DATA*>* pVecButton );
	void HideEntityUI();
	void ShowFrameBuilding( CBuilding* _pEntity );
	void HideFrameBuilding();

	void ShowBuildingOrderUI( const vector<SHORT>& _vecOrderIcon );
	void HideBuildingOrderUI();

	void UpdateOrderRatio( const BYTE& _byRatio );

public:
	void UpdatePlayerResourceData( const wstring& _wstrChangeResourceData );

private:
	void UpdatePlayerMineralData();
	void UpdatePlayerGasData();
	void UpdatePlayerPopulationData();

private:
	void AddDeleteUIList( CUI*& _pDeleteUI );
	void RenderPlayerResourceData();

};

