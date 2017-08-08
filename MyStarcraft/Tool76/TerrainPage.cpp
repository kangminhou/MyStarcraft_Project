// TerrainPage.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool76.h"
#include "TerrainPage.h"
#include "afxdialogex.h"


// CTerrainPage ��ȭ �����Դϴ�.

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


// CTerrainPage �޽��� ó�����Դϴ�.


BOOL CTerrainPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_TileHeightList.AddString( L"0" );
	m_TileHeightList.AddString( L"1" );
	m_TileHeightList.AddString( L"2" );

	m_TileStateList.AddString( L"Idle" );
	m_TileStateList.AddString( L"Can't Go" );

	m_TileHeightList.SetCurSel( 0 );
	m_TileStateList.SetCurSel( 0 );

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CTerrainPage::OnCbnSelchangeComboTileHeight()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString strTemp = L"";
	int iIndex = m_TileHeightList.GetCurSel();
	m_TileHeightList.GetLBText( iIndex, strTemp );

	m_iTileHeight = iIndex;

}

void CTerrainPage::OnCbnSelchangeComboTileState()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString strTemp = L"";
	int iIndex = m_TileStateList.GetCurSel();
	m_TileStateList.GetLBText( iIndex, strTemp );

	m_iTileState = iIndex;

}
