// MiniView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool76.h"
#include "MiniView.h"

#include "Device.h"
#include "BackGround.h"
#include "MainFrm.h"
#include "Tool76View.h"
#include "Mouse.h"
#include "MainFrm.h"

#include "BackGround.h"

// CMiniView

IMPLEMENT_DYNCREATE(CMiniView, CView)

void CMiniView::MoveMapScroll( CPoint ptMouse )
{
	if ( !m_pBackground )
	{
		CMainFrame* pMainFrm = ((CMainFrame*)AfxGetMainWnd());
		if ( !pMainFrm )
			return;

		m_pBackground = pMainFrm->GetMainView()->GetBackGround();
	}

	CTool76View* pMainView = m_pBackground->GetMainView();

	float fMaxX = pMainView->GetScrollLimit( 0 );
	float fMaxY = pMainView->GetScrollLimit( 1 );
	float fScrollX = (float( fMaxX ) / 300.f) * ptMouse.x;
	float fScrollY = (float( fMaxY ) / 200.f) * ptMouse.y;

	pMainView->SetScrollPos( 0, fScrollX );
	pMainView->SetScrollPos( 1, fScrollY );

	pMainView->Invalidate( FALSE );
	Invalidate( FALSE );
}

CMiniView::CMiniView()
	: m_pBackground(NULL), m_bMouseClick(false)
{

}

CMiniView::~CMiniView()
{
}

BEGIN_MESSAGE_MAP(CMiniView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CMiniView 그리기입니다.

void CMiniView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.


	CBackGround* pBackGround = ((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround();

	CDevice::GetInstance()->Render_Begin();

	pBackGround->MiniMapRender();
	
	CDevice::GetInstance()->Render_End(m_hWnd);
}


// CMiniView 진단입니다.

#ifdef _DEBUG
void CMiniView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CMiniView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMiniView 메시지 처리기입니다.


void CMiniView::OnLButtonDown( UINT nFlags, CPoint point )
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnLButtonDown( nFlags, point );

	m_bMouseClick = true;
	
	MoveMapScroll( point );
}


void CMiniView::OnLButtonUp( UINT nFlags, CPoint point )
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnLButtonUp( nFlags, point );

	m_bMouseClick = false;
}

void CMiniView::OnMouseMove( UINT nFlags, CPoint point )
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnMouseMove( nFlags, point );

	if ( m_bMouseClick )
	{
		MoveMapScroll( point );
	}

}
