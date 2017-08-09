// TerrainPage.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool76.h"
#include "TerrainPage.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "Tool76View.h"

#include "BackGround.h"


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
	ON_BN_CLICKED( IDC_BUTTON_SAVE_TILE, &CTerrainPage::OnBnClickedButtonSaveTile )
	ON_BN_CLICKED( IDC_BUTTON_LOAD_TILE, &CTerrainPage::OnBnClickedButtonLoadTile )
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
	m_TileStateList.AddString( L"Can't GoXXXX" );

	m_TileStateList.AddString( L"Can't GoOXXX" );
	m_TileStateList.AddString( L"Can't GoXOXX" );
	m_TileStateList.AddString( L"Can't GoXXOX" );
	m_TileStateList.AddString( L"Can't GoXXXO" );

	m_TileStateList.AddString( L"Can't GoOOXX" );
	m_TileStateList.AddString( L"Can't GoOXOX" );
	m_TileStateList.AddString( L"Can't GoXXOO" );
	m_TileStateList.AddString( L"Can't GoXOXO" );
	m_TileStateList.AddString( L"Can't GoOXXO" );
	m_TileStateList.AddString( L"Can't GoXOOX" );

	m_TileStateList.AddString( L"Can't GoOOOX" );
	m_TileStateList.AddString( L"Can't GoOOXO" );
	m_TileStateList.AddString( L"Can't GoOXOO" );
	m_TileStateList.AddString( L"Can't GoXOOO" );

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


void CTerrainPage::OnBnClickedButtonSaveTile()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CMainFrame* pMainFrm = ((CMainFrame*)AfxGetMainWnd());
	if ( !pMainFrm )
		return;

	CBackGround* pBackGround = pMainFrm->GetMainView()->GetBackGround();

	vector<TILE*>* pVecTile = pBackGround->GetTile();

	DWORD dwByte = 0;

	HANDLE hFile = CreateFile(
		L"../Data/Tile.dat",
		GENERIC_WRITE,	
		NULL,		
		NULL,
		CREATE_ALWAYS,	
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	for(size_t i = 0; i < pVecTile->size(); ++i)
	{
		WriteFile(hFile, (*pVecTile)[i] , sizeof(TILE), &dwByte, NULL);
	}

	CloseHandle(hFile);

}


void CTerrainPage::OnBnClickedButtonLoadTile()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	vector<TILE*>* pVecTile = ((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround()->GetTile();

	DWORD dwByte = 0;

	HANDLE hFile = CreateFile(
		L"../Data/Tile.dat",		
		GENERIC_READ,	
		NULL,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,		//�⺻������ ������ �����϶�¿ɼ�.
		NULL
	);

	for(size_t i = 0; i < pVecTile->size(); ++i)
	{
		safe_delete((*pVecTile)[i]);
	}
	pVecTile->clear();

	while(true)
	{
		TILE* pTileData = new TILE;

		ReadFile(hFile, pTileData, sizeof(TILE), &dwByte, NULL);

		if(dwByte == 0)
		{
			safe_delete(pTileData);
			break;
		}

		pVecTile->push_back(pTileData);
	}

	CloseHandle(hFile);

	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
}
