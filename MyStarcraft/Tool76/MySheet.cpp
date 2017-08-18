// MySheet.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool76.h"
#include "MySheet.h"

#include "TerrainPage.h"
#include "UnitPage.h"
#include "ObjectPage.h"
#include "WeaponPage.h"


// CMySheet

IMPLEMENT_DYNAMIC( CMySheet, CPropertySheet )

CMySheet::CMySheet( UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage )
	:CPropertySheet( nIDCaption, pParentWnd, iSelectPage )
{

}

CMySheet::CMySheet( LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage )
	: CPropertySheet( pszCaption, pParentWnd, iSelectPage )
{

}

CMySheet::CMySheet( void )
{
	m_pPageArr[Terrain] = new CTerrainPage;
	m_pPageArr[Unit] = new CUnitPage;
	m_pPageArr[Object] = new CObjectPage;
	m_pPageArr[Weapon] = new CWeaponPage;

	//[17.07.21_05] : 시트에 페이지를 연결하자.
	AddPage( m_pPageArr[Terrain] );
	AddPage( m_pPageArr[Unit] );
	AddPage( m_pPageArr[Object] );
	AddPage( m_pPageArr[Weapon] );
}

CMySheet::~CMySheet()
{
	for ( int i = 0; i < End; ++i )
	{
		delete m_pPageArr[i];
		m_pPageArr[i] = NULL;
	}
}


BEGIN_MESSAGE_MAP( CMySheet, CPropertySheet )
END_MESSAGE_MAP()


// CMySheet 메시지 처리기입니다.
