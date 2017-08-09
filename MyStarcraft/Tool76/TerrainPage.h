#pragma once
#include "afxwin.h"


// CTerrainPage 대화 상자입니다.

class CTerrainPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CTerrainPage)

private:
	int m_iTileHeight;
	int m_iTileState;

public:
	void SetTileHeight( int _iTileHeight );
	void SetTileState( int _iTileState );

public:
	int GetTileHeight() const { return m_iTileHeight; }
	int GetTileState() const { return m_iTileState; }

public:
	CTerrainPage();
	virtual ~CTerrainPage();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TERRAINPAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeComboTileHeight();
	CComboBox m_TileHeightList;
	CComboBox m_TileStateList;
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboTileState();
	afx_msg void OnBnClickedButtonSaveTile();
	afx_msg void OnBnClickedButtonLoadTile();
};
