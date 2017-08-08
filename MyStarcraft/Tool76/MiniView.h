#pragma once


// CMiniView ���Դϴ�.

class CBackGround;
class CMiniView : public CView
{
	DECLARE_DYNCREATE(CMiniView)

private:
	bool			m_bMouseClick;
	CBackGround*	m_pBackground;

private:
	void MoveMapScroll( CPoint ptMouse );

protected:
	CMiniView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CMiniView();

public:
	virtual void OnDraw(CDC* pDC);      // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown( UINT nFlags, CPoint point );
	afx_msg void OnLButtonUp( UINT nFlags, CPoint point );
	afx_msg void OnMouseMove( UINT nFlags, CPoint point );
};


