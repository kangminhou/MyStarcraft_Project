#pragma once
#include "afxwin.h"


// CMapTool ��ȭ �����Դϴ�.
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
	CMapTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMapTool();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MAPTOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnSelchangeListImagePath();
	CListBox m_listBoxImagePath;
	afx_msg void OnDropFiles(HDROP hDropInfo);
	CStatic m_Picture;
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonLoad();
};
