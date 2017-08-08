
// MainFrm.h : CMainFrame 클래스의 인터페이스
//

#pragma once

class CTool76View;
class CMiniView;
class CMyForm;
class CMainFrame : public CFrameWndEx
{
protected: // serialization에서만 만들어집니다.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

private:		//CSplitterWnd : 화면 분할을 도와주는 클래스.
	CSplitterWnd	m_MainSplitter;
	CSplitterWnd	m_SecondSplitter;	//홀수창 분할을 위해서 추가적으로 사용하는 변수.

private:
	CTool76View* m_pMainView;
	CMiniView*	 m_pMiniView;
	CMyForm*	 m_pMyForm;

public:
	CMiniView*	GetMiniView(void)
	{
		return m_pMiniView;
	}

	CTool76View* GetMainView(void)
	{
		return m_pMainView;
	}

	CMyForm*	GetMyForm(void)
	{
		return m_pMyForm;
	}


// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 컨트롤 모음이 포함된 멤버입니다.
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;

// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
};


