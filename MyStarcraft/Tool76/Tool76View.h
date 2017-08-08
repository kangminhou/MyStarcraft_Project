
// Tool76View.h : CTool76View Ŭ������ �������̽�
//


#pragma once

class CSingleTexture;
class CBackGround;


//class CTool76View : public CView

class CTool76Doc;
//##��ũ�� �ٸ� ����� ���ؼ� ����� ��������.
class CTool76View : public CScrollView
{
private:
	CSingleTexture*			m_pSingleTexture;
	CBackGround*			m_pBackGround;

public:
	CBackGround*	GetBackGround(void)
	{
		return m_pBackGround;
	}

	

protected: // serialization������ ��������ϴ�.
	CTool76View();
	DECLARE_DYNCREATE(CTool76View)

// Ư���Դϴ�.
public:
	CTool76Doc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CTool76View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown( UINT nFlags, CPoint point );
//	afx_msg void OnLButtonUp( UINT nFlags, CPoint point );
};

#ifndef _DEBUG  // Tool76View.cpp�� ����� ����
inline CTool76Doc* CTool76View::GetDocument() const
   { return reinterpret_cast<CTool76Doc*>(m_pDocument); }
#endif

