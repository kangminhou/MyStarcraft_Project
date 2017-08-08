
// MainFrm.h : CMainFrame Ŭ������ �������̽�
//

#pragma once

class CTool76View;
class CMiniView;
class CMyForm;
class CMainFrame : public CFrameWndEx
{
protected: // serialization������ ��������ϴ�.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

private:		//CSplitterWnd : ȭ�� ������ �����ִ� Ŭ����.
	CSplitterWnd	m_MainSplitter;
	CSplitterWnd	m_SecondSplitter;	//Ȧ��â ������ ���ؼ� �߰������� ����ϴ� ����.

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


// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// �����Դϴ�.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // ��Ʈ�� ������ ���Ե� ����Դϴ�.
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;

// ������ �޽��� �� �Լ�
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
};


