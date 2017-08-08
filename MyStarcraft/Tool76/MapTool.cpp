// MapTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool76.h"
#include "MapTool.h"
#include "include.h"

#include "MainFrm.h"
#include "Tool76View.h"
#include "BackGround.h"

void CMapTool::HorizontalScroll(void)
{
	CString strName;
	CSize	Size;

	int iCX = 0;

	CDC* pDc = m_listBoxImagePath.GetDC();
	//리스트박스의 DC를 얻어온다.


	//리스트박스에 있는 목록의 개수를 받아오자.
	for(int i = 0; i < m_listBoxImagePath.GetCount(); ++i)
	{
		//리스트박스에 있는 문자열을 얻어온다.
		m_listBoxImagePath.GetText(i, strName);

		//글자의 크기를 픽셀크기로 변환해주는 함수.
		Size = pDc->GetTextExtent(strName);

		//가장 긴 문자열을 검사하자.
		if(Size.cx > iCX)
		{
			iCX = Size.cx;
		}
	}


	//검색이 완료되면 DC를 반환하자.
	m_listBoxImagePath.ReleaseDC(pDc);

	if(m_listBoxImagePath.GetHorizontalExtent() < iCX)
	{
		m_listBoxImagePath.SetHorizontalExtent(iCX);
		//가장 긴 문자열의 크기대로 스크롤 바를 생성한다.
	}

}

// CMapTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=NULL*/)
	: CDialog(CMapTool::IDD, pParent)
	, m_iDrawID(0)
{

}

CMapTool::~CMapTool()
{
	map<CString, CImage*>::iterator iter = m_mapPngImage.begin();
	map<CString, CImage*>::iterator iter_end = m_mapPngImage.end();

	for(iter; iter != iter_end; ++iter)
	{
		iter->second->Destroy();		//이미지 객체를 파괴하자.
		safe_delete(iter->second);
	}
	if(!m_mapPngImage.empty())
		m_mapPngImage.clear();
}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_IMAGE_PATH, m_listBoxImagePath);
	DDX_Control(pDX, IDC_PICTURE, m_Picture);
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST_IMAGE_PATH, &CMapTool::OnLbnSelchangeListImagePath)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CMapTool::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CMapTool::OnBnClickedButtonLoad)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CMapTool 메시지 처리기입니다.

void CMapTool::OnLbnSelchangeListImagePath()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	CString strName;

	int iSelectIndex = m_listBoxImagePath.GetCurSel();
	
	if(iSelectIndex < 0)
		return;

	m_listBoxImagePath.GetText(iSelectIndex, strName);

	map<CString, CImage*>::iterator iter;

	iter = m_mapPngImage.find(strName);

	if(iter == m_mapPngImage.end())
		return;

	m_Picture.SetBitmap( *iter->second );


	int i = 0;

	for(i = 0; i < strName.GetLength(); ++i)
	{
		//문자열을 0번 인덱스부터 검색
		//문자가 숫자인지를 검사해보자. -> 
		if( isdigit( strName[i] ) != 0 )		//Tile0
		{
			break;
			//함수isdigit()의 반환값이 0이 아닌 수이면 문자가 '숫자'이다.
		}
	}

	//i -> 숫자가 있는곳까지의 Index
	strName.Delete(0, i);
	//0번 인덱스부터 i까지 문자열을 삭제한다.


	//_tstoi() : string to int -> 문자열을 정수형으로 변경하는 함수.
	m_iDrawID = _tstoi(strName);	//strName = Tile0

	UpdateData(FALSE);
}

void CMapTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.


	////##[17.07.18_00] : 드롭된 파일의 개수를 받아오자.

	UpdateData(TRUE);

	int iFileCount = DragQueryFile(hDropInfo, -1, NULL, 0);
	//1인자 : 드롭된 파일을 나타내는 구조를 요구하고 있다.

	//2인자 : 파일의 인덱스를 지정하는 인자값.
	//        -1을 인자값으로 사용하게 된다면 파일 전체개수를 반환한다.

	//3인자 : 문자열의 포인터를 담는 인자.

	//4인자 : 지정한 버퍼사이즈를 UniCode인 경우에 문자의 수로 지정을 한다.
	

	TCHAR szFullPath[MAX_PATH] = L"";			//드롭된 파일의 경로를 보관할 변수.

	for(int i = 0; i < iFileCount; ++i)
	{
		DragQueryFile(hDropInfo, i, szFullPath, MAX_PATH);

		std::wstring wstrRelativePath = RelativePath(szFullPath);

		CString strImageName;

		//##[17.07.18_02]
		//파일의 이름만 얻어오는 함수.
		strImageName = PathFindFileName(wstrRelativePath.c_str());


		//확장자를 제거해주는 함수.
		PathRemoveExtension( (LPWSTR)strImageName.operator LPCWSTR() );
		

		//map컨테이너 관리.
		map<CString, CImage*>::iterator iter;

		iter = m_mapPngImage.find(strImageName);

		if(iter == m_mapPngImage.end())
		{
			CImage* pPngImage = new CImage;
			
			//PNG이미지를 상대경로를 통해서 이미지를 로드한다.
			pPngImage->Load(wstrRelativePath.c_str());

			//키 : 파일이름(ex.Tile0)   
			m_mapPngImage.insert( make_pair(strImageName, pPngImage) );
		}


		m_listBoxImagePath.AddString(strImageName);
	}


	HorizontalScroll();
	

	CDialog::OnDropFiles(hDropInfo);


	UpdateData(FALSE);
}

void CMapTool::OnBnClickedButtonSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	vector<TILE*>* pVecTile = ((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround()->GetTile();

	DWORD dwByte = 0;

	HANDLE hFile = CreateFile(
		L"../Data/Tile.dat",		
		GENERIC_WRITE,	
		NULL,		
		NULL,
		CREATE_ALWAYS,	
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);

	for(size_t i = 0; i < pVecTile->size(); ++i)
	{
		WriteFile(hFile, (*pVecTile)[i] , sizeof(TILE), &dwByte, NULL);
	}

	CloseHandle(hFile);
}

void CMapTool::OnBnClickedButtonLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.


	vector<TILE*>* pVecTile = ((CMainFrame*)AfxGetMainWnd())->GetMainView()->GetBackGround()->GetTile();

	DWORD dwByte = 0;

	HANDLE hFile = CreateFile(
		L"../Data/Tile.dat",		
		GENERIC_READ,	
		NULL,		
		NULL,
		OPEN_EXISTING,	
		FILE_ATTRIBUTE_NORMAL,		//기본값으로 파일을 생성하라는옵션.
		NULL
		);

	for(size_t i = 0; i < pVecTile->size(); ++i)
	{
		safe_delete((*pVecTile)[i]);
	}
	pVecTile->clear();

	while(true)
	{
		TILE* pTileData = new TILE;

		ReadFile(hFile, pTileData, sizeof(TILE), &dwByte, NULL);

		if(dwByte == 0)
		{
			safe_delete(pTileData);
			break;
		}

		pVecTile->push_back(pTileData);
	}

	CloseHandle(hFile);

	((CMainFrame*)AfxGetMainWnd())->GetMainView()->Invalidate(FALSE);
}
