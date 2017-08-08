
// Tool76View.h : CTool76View 클래스의 인터페이스
//


#pragma once

class CSingleTexture;
class CBackGround;


//class CTool76View : public CView

class CTool76Doc;
//##스크롤 바를 만들기 위해서 상속을 변경하자.
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

	

protected: // serialization에서만 만들어집니다.
	CTool76View();
	DECLARE_DYNCREATE(CTool76View)

// 특성입니다.
public:
	CTool76Doc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CTool76View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
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

#ifndef _DEBUG  // Tool76View.cpp의 디버그 버전
inline CTool76Doc* CTool76View::GetDocument() const
   { return reinterpret_cast<CTool76Doc*>(m_pDocument); }
#endif

