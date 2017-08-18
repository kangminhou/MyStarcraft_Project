#pragma once



// CMySheet

class CPropertyPage;

class CMySheet : public CPropertySheet
{
	DECLARE_DYNAMIC( CMySheet )

	//[17.07.21_04]
	//�������� ��������� ������ ����.

public:
	enum ePageKind { Terrain, Unit, Object, Weapon, End };

private:
	CPropertyPage*	m_pPageArr[End];

public:
	CPropertyPage*	GetPage( ePageKind eKind ) { return m_pPageArr[eKind]; }

public:				//[17.07.21_01]
	CMySheet( void );	//���⺻ �����ڸ� ������ �츮�� ���Կ����� �ʿ����.
	CMySheet( UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0 );
	CMySheet( LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0 );
	virtual ~CMySheet();

protected:
	DECLARE_MESSAGE_MAP()
};


