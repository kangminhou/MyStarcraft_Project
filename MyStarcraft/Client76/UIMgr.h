#pragma once
#include "Include.h"

class CUI;
class CGameEntity;
class CCorps;
class CMyButton;
class CPlayer;

class CUIMgr
{
	DECLARE_SINGLETON(CUIMgr)

private:
	CUI*	m_pMainUI;

	CUI*	m_pFaceUI;
	CUI*	m_pCorpsWireFrameUIArr[12];
	CUI*	m_pEntityWireFrameUI;

	CUI*	m_pUnitUpgradeUI[3];
	CUI*	m_pUnitUpgradeFontUI[3];
	CUI*	m_pUnitUpgradeBackUI[3];

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

	BYTE	m_byDrawUINum;
	BYTE	m_byDrawHpUINum;

	bool	m_bShowEntityUI;
	bool	m_bShowCorpsWire;

public:
	CUIMgr();
	~CUIMgr();

public:
	void SetPlayer( CPlayer* _pPlayer );

public:
	void Initialize();
	void Update();
	void Render();
	void Release();
	
public:
	void ShowEntityUI( CCorps* _pCorps );
	void ShowButton( vector<BUTTON_DATA*>* pVecButton );
	void HideEntityUI();

private:
	void AddDeleteUIList( CUI*& _pDeleteUI );

};

