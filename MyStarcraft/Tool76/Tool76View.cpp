
// Tool76View.cpp : CTool76View 클래스의 구현
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

//##스크롤 바 상속 처리.
IMPLEMENT_DYNCREATE(CTool76View, CScrollView)						//변경

BEGIN_MESSAGE_MAP(CTool76View, CScrollView)							//변경
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)			//변경
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)		//변경
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CTool76View::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
//	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CTool76View 생성/소멸

CTool76View::CTool76View()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CTool76View::~CTool76View()
{
	CDevice::GetInstance()->DestroyInstance();
}

BOOL CTool76View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CTool76View 그리기
/*
void CTool76View::OnDraw(CDC* pDC)
{
	CTool76Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.

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

	//그림을 그린다.
	CDevice::GetInstance()->GetSprite()->Draw(
		pTileTex->pTexture,
		NULL,	//보여줄 이미지의 크기
		&D3DXVECTOR3(fX, fY, 0.f),	//이미지의 센터
		NULL,	//이미지의 출력 위치
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


// CTool76View 인쇄


void CTool76View::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CTool76View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CTool76View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CTool76View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
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


// CTool76View 진단

#ifdef _DEBUG
void CTool76View::AssertValid() const
{
	CView::AssertValid();
}

void CTool76View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTool76Doc* CTool76View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTool76Doc)));
	return (CTool76Doc*)m_pDocument;
}
#endif //_DEBUG


// CTool76View 메시지 처리기

void CTool76View::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	SetScrollSizes(MM_TEXT, CSize(TILECX * TILEX, TILECY * TILEY) );

	//MM_TEXT	  : 1:1 픽셀단위로 처리하는 옵션.
	//MM_HIMETRIC : 0.01mm


	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	//AfxGetMainWnd() : 창 테두리를 얻어오는 함수이다.
	//반환타입은 CWnd(부모)포인터형이므로 자식객체의 타입으로 
	//형변환을 해서 사용한다.

	RECT rcWindow;
	

	pMainFrame->GetWindowRect(&rcWindow);
	//전체 프레임의 사이즈를 얻어오는 함수.


	SetRect(&rcWindow, 
		0, 
		0, 
		rcWindow.right - rcWindow.left, 
		rcWindow.bottom - rcWindow.top);
	//프레임의 크기를 0,0기준으로 재조정해주기 위해서 ...


	RECT rcMainView;
	GetClientRect(&rcMainView);
	//순수 뷰창의 크기를 얻어오는 함수.


	//가로 프레임의 길이를 구하자.
	float fX = float(rcWindow.right - rcMainView.right);

	//세로 프레임의 길이를 구하자.
	float fY = float(rcWindow.bottom - rcMainView.bottom);

	pMainFrame->SetWindowPos(
		NULL,		//프레임의 크기를 계산한 상태로 출력할 위치를 결정하는 인자.
		0,
		0,
		int(WINCX + fX),
		int(WINCY + fY),
		SWP_NOZORDER
		);


	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	g_hWnd = m_hWnd;


	if(FAILED(CDevice::GetInstance()->Init3D() ) )
	{
		AfxMessageBox(L"장치 생성 실패!");
	}

	//m_pSingleTexture =  new CSingleTexture;
	//m_pSingleTexture->InsertTexture(L"../Texture/Cube1.png");


	if( FAILED(CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Cube1.png"
		, L"Cube"
		, TEX_SINGLE)))
	{
		AfxMessageBox(L"싱글 이미지 로드 실패!");
	}

	if( FAILED(CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Tile/DebugTile%d.png"
		, L"Back",
		TEX_MULTI,
		L"Tile",
		2,
		true,
		D3DCOLOR_ARGB( 255, 255, 255, 255 ) )))
	{
		AfxMessageBox(L"싱글 이미지 로드 실패!");
	}

	if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Map/Map.png", L"BackGround", TEX_SINGLE ) ) )
	{
		AfxMessageBox( L"맵 이미지 로드 실패!" );
	}

	if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Map/MiniMapRect.png", L"MinimapArea", TEX_SINGLE ) ) )
	{
		AfxMessageBox( L"맵 이미지 로드 실패!" );
	}

	m_pBackGround = new CBackGround;
	m_pBackGround->Initialize();

	m_pBackGround->SetMainView(this);
}

void CTool76View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

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
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

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
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnMouseMove(nFlags, point);

	if ( GetAsyncKeyState( VK_LBUTTON ) )
		OnLButtonDown( nFlags, point );

	if ( GetAsyncKeyState( VK_RBUTTON ) )
		OnRButtonDown( nFlags, point );
}


void CTool76View::OnRButtonDown( UINT nFlags, CPoint point )
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

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
//	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
//
//	CScrollView::OnLButtonUp( nFlags, point );
//}
