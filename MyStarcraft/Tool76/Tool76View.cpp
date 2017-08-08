
// Tool76View.cpp : CTool76View Ŭ������ ����
//

#include "stdafx.h"
#include "Tool76.h"

#include "Tool76Doc.h"
#include "Tool76View.h"
#include "Include.h"

#include "MainFrm.h"

#include "Device.h"

#include "SingleTexture.h"

#include "TextureMgr.h"
#include "BackGround.h"

#include "Mouse.h"

#include "MiniView.h"
#include "MapTool.h"
#include "MyForm.h"

#include "MySheet.h"
#include "TerrainPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTool76View

//##��ũ�� �� ��� ó��.
IMPLEMENT_DYNCREATE(CTool76View, CScrollView)						//����

BEGIN_MESSAGE_MAP(CTool76View, CScrollView)							//����
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)			//����
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)		//����
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CTool76View::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
//	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CTool76View ����/�Ҹ�

CTool76View::CTool76View()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CTool76View::~CTool76View()
{
	CDevice::GetInstance()->DestroyInstance();
}

BOOL CTool76View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CTool76View �׸���
/*
void CTool76View::OnDraw(CDC* pDC)
{
	CTool76Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.

	//Rectangle()

	//pDC->Rectangle(100, 100, 200, 200);

	D3DXMATRIX matWorld, matTrans, matScale, matRotZ;

	D3DXMatrixIdentity(&matWorld);

	D3DXMatrixScaling(&matScale, 1, 1, 1);
	D3DXMatrixTranslation(&matTrans, 400, 300, 0);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(0));

	matWorld = matScale * matRotZ * matTrans;

	CDevice::GetInstance()->Render_Begin();

	CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

	//const TEX_INFO* pCubeTexture = CTextureMgr::GetInstance()->GetTexture(L"Cube");

	const TEX_INFO* pTileTex = CTextureMgr::GetInstance()->GetTexture(L"Back", L"Tile", 3);

	float fX = CENTER_POS(pTileTex->ImageInfo.Width);
	float fY = CENTER_POS(pTileTex->ImageInfo.Height);

	//�׸��� �׸���.
	CDevice::GetInstance()->GetSprite()->Draw(
		pTileTex->pTexture,
		NULL,	//������ �̹����� ũ��
		&D3DXVECTOR3(fX, fY, 0.f),	//�̹����� ����
		NULL,	//�̹����� ��� ��ġ
		D3DCOLOR_ARGB(255, 255, 255, 255)
		);
	
	CDevice::GetInstance()->Render_End();

}
*/

void CTool76View::OnDraw(CDC* pDC)
{
	CTool76Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	
	CDevice::GetInstance()->Render_Begin();

	m_pBackGround->Render();
	
	CDevice::GetInstance()->Render_End();

}


// CTool76View �μ�


void CTool76View::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CTool76View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CTool76View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CTool76View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}

void CTool76View::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CTool76View::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CTool76View ����

#ifdef _DEBUG
void CTool76View::AssertValid() const
{
	CView::AssertValid();
}

void CTool76View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTool76Doc* CTool76View::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTool76Doc)));
	return (CTool76Doc*)m_pDocument;
}
#endif //_DEBUG


// CTool76View �޽��� ó����

void CTool76View::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	SetScrollSizes(MM_TEXT, CSize(TILECX * TILEX, TILECY * TILEY) );

	//MM_TEXT	  : 1:1 �ȼ������� ó���ϴ� �ɼ�.
	//MM_HIMETRIC : 0.01mm


	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	//AfxGetMainWnd() : â �׵θ��� ������ �Լ��̴�.
	//��ȯŸ���� CWnd(�θ�)���������̹Ƿ� �ڽİ�ü�� Ÿ������ 
	//����ȯ�� �ؼ� ����Ѵ�.

	RECT rcWindow;
	

	pMainFrame->GetWindowRect(&rcWindow);
	//��ü �������� ����� ������ �Լ�.


	SetRect(&rcWindow, 
		0, 
		0, 
		rcWindow.right - rcWindow.left, 
		rcWindow.bottom - rcWindow.top);
	//�������� ũ�⸦ 0,0�������� ���������ֱ� ���ؼ� ...


	RECT rcMainView;
	GetClientRect(&rcMainView);
	//���� ��â�� ũ�⸦ ������ �Լ�.


	//���� �������� ���̸� ������.
	float fX = float(rcWindow.right - rcMainView.right);

	//���� �������� ���̸� ������.
	float fY = float(rcWindow.bottom - rcMainView.bottom);

	pMainFrame->SetWindowPos(
		NULL,		//�������� ũ�⸦ ����� ���·� ����� ��ġ�� �����ϴ� ����.
		0,
		0,
		int(WINCX + fX),
		int(WINCY + fY),
		SWP_NOZORDER
		);


	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	g_hWnd = m_hWnd;


	if(FAILED(CDevice::GetInstance()->Init3D() ) )
	{
		AfxMessageBox(L"��ġ ���� ����!");
	}

	//m_pSingleTexture =  new CSingleTexture;
	//m_pSingleTexture->InsertTexture(L"../Texture/Cube1.png");


	if( FAILED(CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Cube1.png"
		, L"Cube"
		, TEX_SINGLE)))
	{
		AfxMessageBox(L"�̱� �̹��� �ε� ����!");
	}

	if( FAILED(CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Tile/DebugTile%d.png"
		, L"Back",
		TEX_MULTI,
		L"Tile",
		2,
		true,
		D3DCOLOR_ARGB( 255, 255, 255, 255 ) )))
	{
		AfxMessageBox(L"�̱� �̹��� �ε� ����!");
	}

	if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Map/Map.png", L"BackGround", TEX_SINGLE ) ) )
	{
		AfxMessageBox( L"�� �̹��� �ε� ����!" );
	}

	if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Map/MiniMapRect.png", L"MinimapArea", TEX_SINGLE ) ) )
	{
		AfxMessageBox( L"�� �̹��� �ε� ����!" );
	}

	m_pBackGround = new CBackGround;
	m_pBackGround->Initialize();

	m_pBackGround->SetMainView(this);
}

void CTool76View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CView::OnLButtonDown(nFlags, point);

	D3DXVECTOR3 vMousePos = D3DXVECTOR3( CMouse::GetMousePos().x + GetScrollPos( 0 ), CMouse::GetMousePos().y + GetScrollPos( 1 ), 0.f );
	
	//CMapTool* pMapTool = ((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetMapTool();
	CMySheet*	pMySheet = ((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetSheet();
	CTerrainPage* pTerrainPage = dynamic_cast<CTerrainPage*>(pMySheet->GetPage( CMySheet::Terrain ));

	m_pBackGround->TileChange(vMousePos, pTerrainPage->GetTileState(), pTerrainPage->GetTileHeight() );

//	CMiniView* pMiniView = ((CMainFrame*)AfxGetMainWnd())->GetMiniView();
//	pMiniView->Invalidate(FALSE);

	((CMainFrame*)AfxGetMainWnd())->GetMiniView()->Invalidate(FALSE);

	Invalidate(FALSE);
	//Invalidate(TRUE);
}

void CTool76View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CView::OnKeyDown(nChar, nRepCnt, nFlags);

	switch(nChar)
	{
	case VK_ESCAPE:
		//((CMainFrame*)AfxGetMainWnd())->DestroyWindow();
		dynamic_cast<CMainFrame*>(AfxGetMainWnd())->DestroyWindow();
		break;
	}
}

void CTool76View::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CScrollView::OnMouseMove(nFlags, point);

	if ( GetAsyncKeyState( VK_LBUTTON ) )
		OnLButtonDown( nFlags, point );

	if ( GetAsyncKeyState( VK_RBUTTON ) )
		OnRButtonDown( nFlags, point );
}


void CTool76View::OnRButtonDown( UINT nFlags, CPoint point )
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CScrollView::OnRButtonDown( nFlags, point );
	D3DXVECTOR3 vMousePos = D3DXVECTOR3( CMouse::GetMousePos().x + GetScrollPos( 0 ), CMouse::GetMousePos().y + GetScrollPos( 1 ), 0.f );

	//CMapTool* pMapTool = ((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetMapTool();
	CMySheet*	pMySheet = ((CMainFrame*)AfxGetMainWnd())->GetMyForm()->GetSheet();
	CTerrainPage* pTerrainPage = dynamic_cast<CTerrainPage*>(pMySheet->GetPage( CMySheet::Terrain ));

	int iIndex = int(vMousePos.x / TILECX) + int(vMousePos.y / TILECY) * TILEX;
	if ( iIndex < 0 || iIndex >= TILEX * TILEY )
		return;

	TILE* pTile = (*m_pBackGround->GetTile())[iIndex];
	pTerrainPage->SetTileHeight( pTile->byHeight );
	pTerrainPage->SetTileState( pTile->byOption );

	((CMainFrame*)AfxGetMainWnd())->GetMiniView()->Invalidate(FALSE);

	Invalidate(FALSE);
}


//void CTool76View::OnLButtonUp( UINT nFlags, CPoint point )
//{
//	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
//
//	CScrollView::OnLButtonUp( nFlags, point );
//}
