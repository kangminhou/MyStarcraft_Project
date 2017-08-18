#pragma once



// CMySheet

class CPropertyPage;

class CMySheet : public CPropertySheet
{
	DECLARE_DYNAMIC( CMySheet )

	//[17.07.21_04]
	//페이지를 멤버변수로 가지고 있자.

public:
	enum ePageKind { Terrain, Unit, Object, Weapon, End };

private:
	CPropertyPage*	m_pPageArr[End];

public:
	CPropertyPage*	GetPage( ePageKind eKind ) { return m_pPageArr[eKind]; }

public:				//[17.07.21_01]
	CMySheet( void );	//갹기본 생성자를 만들자 우리는 대입연산이 필요없다.
	CMySheet( UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0 );
	CMySheet( LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0 );
	virtual ~CMySheet();

protected:
	DECLARE_MESSAGE_MAP()
};


