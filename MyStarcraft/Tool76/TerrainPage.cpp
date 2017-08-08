// TerrainPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool76.h"
#include "TerrainPage.h"
#include "afxdialogex.h"


// CTerrainPage 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTerrainPage, CPropertyPage)

void CTerrainPage::SetTileHeight( int _iTileHeight )
{
	if ( _iTileHeight >= 0 )
		m_iTileHeight = _iTileHeight;

	m_TileHeightList.SetCurSel( _iTileHeight );
}

void CTerrainPage::SetTileState( int _iTileState )
{
	if ( _iTileState >= 0 )
		m_iTileState = _iTileState;

	m_TileStateList.SetCurSel( _iTileState );
}

CTerrainPage::CTerrainPage()
	: CPropertyPage(IDD_TERRAINPAGE), m_iTileHeight(0), m_iTileState(0)
{

}

CTerrainPage::~CTerrainPage()
{
}

void CTerrainPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_COMBO_TILE_HEIGHT, m_TileHeightList );
	DDX_Control( pDX, IDC_COMBO_TILE_STATE, m_TileStateList );
}


BEGIN_MESSAGE_MAP(CTerrainPage, CPropertyPage)
	ON_CBN_SELCHANGE( IDC_COMBO_TILE_HEIGHT, &CTerrainPage::OnCbnSelchangeComboTileHeight )
	ON_CBN_SELCHANGE( IDC_COMBO_TILE_STATE, &CTerrainPage::OnCbnSelchangeComboTileState )
END_MESSAGE_MAP()


// CTerrainPage 메시지 처리기입니다.


BOOL CTerrainPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_TileHeightList.AddString( L"0" );
	m_TileHeightList.AddString( L"1" );
	m_TileHeightList.AddString( L"2" );

	m_TileStateList.AddString( L"Idle" );
	m_TileStateList.AddString( L"Can't Go" );

	m_TileHeightList.SetCurSel( 0 );
	m_TileStateList.SetCurSel( 0 );

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CTerrainPage::OnCbnSelchangeComboTileHeight()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strTemp = L"";
	int iIndex = m_TileHeightList.GetCurSel();
	m_TileHeightList.GetLBText( iIndex, strTemp );

	m_iTileHeight = iIndex;

}

void CTerrainPage::OnCbnSelchangeComboTileState()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strTemp = L"";
	int iIndex = m_TileStateList.GetCurSel();
	m_TileStateList.GetLBText( iIndex, strTemp );

	m_iTileState = iIndex;

}
