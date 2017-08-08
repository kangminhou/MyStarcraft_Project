#pragma once
#include "afxwin.h"


// CMapTool 대화 상자입니다.
#include "Include.h"

class CMapTool : public CDialog
{
	DECLARE_DYNAMIC(CMapTool)

private:
	map<CString, CImage*>		m_mapPngImage;
	
public:
	int		m_iDrawID;				

public:
	void HorizontalScroll(void);

public:
	CMapTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMapTool();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MAPTOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnSelchangeListImagePath();
	CListBox m_listBoxImagePath;
	afx_msg void OnDropFiles(HDROP hDropInfo);
	CStatic m_Picture;
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonLoad();
};
