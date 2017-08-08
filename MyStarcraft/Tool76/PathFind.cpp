// PathFind.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool76.h"
#include "PathFind.h"


// CPathFind 대화 상자입니다.

IMPLEMENT_DYNAMIC( CPathFind, CDialog )

void CPathFind::Release( void )
{
	for ( list<IMAGE_PATH*>::iterator iter = m_listImagePath.begin();
		  iter != m_listImagePath.end(); ++iter )
	{
		safe_delete( *iter );
	}
	m_listImagePath.clear();
}

CPathFind::CPathFind( CWnd* pParent /*=NULL*/ )
	: CDialog( CPathFind::IDD, pParent )
{

}

CPathFind::~CPathFind()
{
	Release();
}

void CPathFind::DoDataExchange( CDataExchange* pDX )
{
	CDialog::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_LIST_PATH, m_listboxPathFind );
}


BEGIN_MESSAGE_MAP( CPathFind, CDialog )
	ON_LBN_SELCHANGE( IDC_LIST_PATH, &CPathFind::OnLbnSelchangeListPath )
	ON_BN_CLICKED( IDC_BUTTON_SAVE, &CPathFind::OnBnClickedButtonSave )
	ON_BN_CLICKED( IDC_BUTTON_LOAD, &CPathFind::OnBnClickedButtonLoad )
	ON_WM_DROPFILES()
END_MESSAGE_MAP()




void CPathFind::OnLbnSelchangeListPath()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CPathFind::OnBnClickedButtonSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.


	//[17.07.24_00] : C++입출력 객체를 통해서 파일저장/로드

	wofstream	SaveFile;
	//C++ 입출력 객체.


	SaveFile.open( L"../Data/ImagePath.txt", ios::out );
	//fopen을 대신해서 사용하는 함수.
	//ios::out -> Txt파일.

	int iCount = 0;

	for ( list<IMAGE_PATH*>::iterator iter = m_listImagePath.begin();
		  iter != m_listImagePath.end(); ++iter )
	{
		++iCount;

		SaveFile << (*iter)->wstrObjKey.c_str() << L"|";
		SaveFile << (*iter)->wstrStateKey.c_str() << L"|";
		SaveFile << (*iter)->iCount << L"|";

		if ( iCount == m_listImagePath.size() )	//파일의 끝.
			SaveFile << (*iter)->wstrPath.c_str() << flush;
		else
			SaveFile << (*iter)->wstrPath.c_str() << endl;
	}

	SaveFile.close();

	WinExec( "notepad.exe ../Data/ImagePath.txt", SW_SHOW );
	//WinExec : 프로세스가 동작중에 다른 파일을 실행시켜줄수 있는 함수.
}

void CPathFind::OnBnClickedButtonLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData( TRUE );

	wifstream LoadFile;

	LoadFile.open( "../Data/ImagePath.txt", ios::in );

	TCHAR szObjKey[MIN_STR] = L"";
	TCHAR szStateKey[MIN_STR] = L"";
	TCHAR szCount[MIN_STR] = L"";
	TCHAR szPath[MAX_PATH] = L"";

	wstring wstrCombine = L"";

	m_listboxPathFind.ResetContent();		//리스트박스 삭제.

	Release();

	while ( !LoadFile.eof() )
	{
		//저장한 라인을 통째로 읽어오는 함수.
		LoadFile.getline( szObjKey, MIN_STR, '|' );
		LoadFile.getline( szStateKey, MIN_STR, '|' );
		LoadFile.getline( szCount, MIN_STR, '|' );
		LoadFile.getline( szPath, MAX_PATH );

		wstrCombine = wstring( szObjKey ) + L"|";
		wstrCombine += wstring( szStateKey ) + L"|"
			+ wstring( szCount ) + L"|"
			+ wstring( szPath );

		m_listboxPathFind.AddString( wstrCombine.c_str() );
	}

	LoadFile.close();

	UpdateData( FALSE );

}

void CPathFind::OnDropFiles( HDROP hDropInfo )
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnDropFiles( hDropInfo );


	UpdateData( TRUE );

	int iFileCount = DragQueryFile( hDropInfo, -1, NULL, NULL );

	TCHAR szFullPath[MAX_PATH] = L"";


	for ( int i = 0; i < iFileCount; ++i )
	{
		DragQueryFile( hDropInfo, i, szFullPath, MAX_PATH );

		DirectoryInfoExtraction( szFullPath, m_listImagePath );

		wstring wstrCombine = L"";		//하나의 문자열로 보관하기 위한 임시변수.

		TCHAR szIndex[MIN_STR] = L"";	//정수를 문자열형태로 보관하기 위한 변수.

		for ( list<IMAGE_PATH*>::iterator iter = m_listImagePath.begin();
			  iter != m_listImagePath.end(); ++iter )
		{
			//정수를 문자로 변경하자.
			_itow_s( (*iter)->iCount, szIndex, 10 );

			wstrCombine = (*iter)->wstrObjKey + L"|" +
				(*iter)->wstrStateKey + L"|";

			wstrCombine += szIndex;

			wstrCombine += L"|" + (*iter)->wstrPath;

			m_listboxPathFind.AddString( wstrCombine.c_str() );
		}


	}

	UpdateData( FALSE );
}
