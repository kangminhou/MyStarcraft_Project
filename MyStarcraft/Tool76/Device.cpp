#include "StdAfx.h"
#include "Device.h"

//##특정 성능에 대한 장치의 정보를 알아보자.
//1.특정 정보 : 버텍스 프로세싱이라는 것이다.
//   ->다이렉트에서는 실사와 비슷하게 표현하기 위해서
//     어떤 객체의 반사하는 색상을 모두 정의해 놓는다.

//'버텍스'?
//점(.)이다.
//다이렉트에서 이미지가 늘어나도 회전이 가능한 이유는
//점들의 변환이 있기 때문에 가능하다.

//정리 : 버텍스 프로세싱 -> 점의변환 + 조명

//void Test(int*& pTes)
//{
//
//}
//
//void Test(int** ppTes)
//{
//	*ppTes = 10;
//}


IMPLEMENT_SINGLETON(CDevice);

HRESULT CDevice::Init3D(void)
{
	//장치를 자체적으로 할당해준다.
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	
	//장치의 성능을 조사해주는 변수를 설정한다.
	D3DCAPS9	DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	if( FAILED( m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps) ) )
	{
		//Afx : MFC전용함수.
		AfxMessageBox(L"장치 생성 실패!");
		return E_FAIL;
	}


	DWORD vp;

	//결과값에 따라서
	//점과 조명에 대한 연산을 하드웨어(그래픽카드)가 할지? 소프트웨어(CPU)가 할지? 판단해준다.
	
	if(DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;		//그래픽 카드를 지원한다면 그래픽카드에서.
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;		//그래픽 카드를 지원X -> CPU

	
	//장치를 생성할때 개발자가 설정해줘야하는 구조체.
	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	SerParameters(d3dpp);		//장치를 설정하는 기능을 가진 함수.
	//즉 이함수는 디바이스를 초기화하는 목적으로 사용할것이다.

	//D3DDEVTYPE_HAL : 하드웨어 추상화 계층..

	if(FAILED( m_pD3D->CreateDevice(D3DADAPTER_DEFAULT
		, D3DDEVTYPE_HAL
		, g_hWnd
		, vp
		, &d3dpp
		, &m_pD3dDevice) ) )
	{
		//m_pD3dDevice : (디바이스객체) 화면에 그림을 그리기 위한 객체.
		AfxMessageBox(L"디바이스 생성 실패!");
		return E_FAIL;
	}


	//스프라이트 객체를 만들어 준다.
	if(FAILED(D3DXCreateSprite(m_pD3dDevice, &m_pSprite)) )
	{
		AfxMessageBox(L"스프라이트객체 생성 실패!");
		return E_FAIL;
	}


	//폰트 객체를 만들어 주자.
	//그전에 폰트 객체를 먼저 초기화(설정값지정).
	D3DXFONT_DESC FontInfo;
	ZeroMemory(&FontInfo, sizeof(FontInfo));


	FontInfo.Height = 20;
	FontInfo.Width = 10;
	FontInfo.Weight = FW_BOLD;
	FontInfo.CharSet = HANGEUL_CHARSET;
	
	lstrcpy(FontInfo.FaceName, L"바탕체");

	
	if(FAILED( D3DXCreateFontIndirect(m_pD3dDevice, &FontInfo, &m_pFont) ))
	{
		AfxMessageBox(L"폰트 객체 생성 실패!");
		return E_FAIL;
	}


	return S_OK;
}

void CDevice::SerParameters(D3DPRESENT_PARAMETERS& d3dp)
{
	d3dp.BackBufferWidth  = WINCX;			//백버퍼의 가로 크기
	d3dp.BackBufferHeight = WINCY;			//백버퍼의 세로 크기
	d3dp.BackBufferCount  = 1;				//백버퍼의 개수.
	d3dp.BackBufferFormat = D3DFMT_A8R8G8B8;		
	//백버퍼는 픽셀단위로 이루어져 있다.
	//32비트를 사용하게 되는데 32비트를 어떻게? 사용하겠느냐?
	

	d3dp.MultiSampleType = D3DMULTISAMPLE_NONE;
	//다중샘플링 기능을 사용하지 않겠다.
	
	//포토샵에서 이미지를 축소 또는 확대...

	//다이렉트에서 축소 또는 확대할때 테두리에 잔상이 남는 경우가 있는데
	//잔상을 잡아주는 보간을 사용하지 않겠다.
	//비용이 많이 ...

	d3dp.MultiSampleQuality = 0;
	//사용하지 않기 때문에 0을 넣어준다.

	
	d3dp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	//백버퍼의 더블 버퍼링 방식을 말한다.

	//버퍼 : 내가 무엇인가 저장시킬수 있는 공간을 말한다.

	//내부적으로 더블버퍼링을 해주는 포인터가 존재를 한다.
	//각다 버퍼를 가리키고 있다.

	//그 포인터를 통해서 각 버퍼의 픽셀에 적근해서 색상을 변경시켜줄수도 있다.
	//포인터 변수의 주소만 변경을 해서 더블버퍼링효과를 가지게 된다.

	//"스왑체인"

	d3dp.hDeviceWindow = g_hWnd;		//윈도우 핸들
	d3dp.Windowed	   = TRUE;			//전체화면:FALSE / 창모드:TRUE


	d3dp.EnableAutoDepthStencil = TRUE;
	//다이렉트3D에서 자동으로 깊이/스텐실버퍼를 만들고 관리하기를 원한다면 TRUE

	d3dp.AutoDepthStencilFormat = D3DFMT_D24S8;
	//깊이버퍼가 관리되는 픽셀의 범위를 지정하는 옵션.

	d3dp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	//전체화면모드롤 출력하게 될때 모니터의 주사율을 따라가도록 설정하는 옵션이다.
	//주사율

	d3dp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	//주사율과 프레임 사이의 간격을 어떻게 할것인가?

	//D3DPRESENT_INTERVAL_IMMEDIATE : 즉시 화면으로 처리를 한다.
	//D3DPRESENT_INTERVAL_DEFAULT : 화면처리하는 간격을 다이렉트에서 결정하게 된다.
}

//1.버퍼를 지운다. 2.그린다. 3.전면버퍼 출력한다.
void CDevice::Render_Begin(void)
{
	//후면 버퍼를 지워준다.
	m_pD3dDevice->Clear(0,		//버퍼의 개수
						NULL,	//버퍼를 삭제할때 삭제하고 싶은 위치값이.
						D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER,	
						//어디부분을 삭제할것인지? -> 후면버퍼, 스텐실, 깊이
						D3DCOLOR_ARGB(255, 0, 0, 255),	//삭제후 표기되는 색상의 값을 지정.
						1.f,		//삭제후 깊이 버퍼의 초기값.
						0			//스텐실 버퍼의 초기값.
						);

	m_pD3dDevice->BeginScene();
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void CDevice::Render_End(void)
{
	//전면버퍼를 출락한다.
	m_pSprite->End();
	m_pD3dDevice->EndScene();
	m_pD3dDevice->Present(NULL, NULL, NULL, NULL);
	//1,2인자 : 렉트값을 요구하고 있다.
	//		    두 렉트값의 비율차이만큼 화면을 축소/확대
	//		    1번 작은렉트  2번 큰렉트 -> 확대
	//			1번 큰렉      2전 작은 렉트 -> 축소

	//3인자 : 어떤 윈도우(핸들)에서 처리할것인가?

	//4인자 : 화면에서 보여지지 않을 영역을 설정할수 있다.
}

void CDevice::Render_End(HWND& _hWnd)
{
	m_pSprite->End();
	m_pD3dDevice->EndScene();
	m_pD3dDevice->Present(NULL, NULL, _hWnd, NULL);
}

void CDevice::Release(void)
{
	//해제 순서는 반드시 기억하고 있자!!
	m_pSprite->Release();
	m_pD3dDevice->Release();
	m_pD3D->Release();
}

CDevice::CDevice(void)
:m_pD3D(NULL)
,m_pD3dDevice(NULL)
{
}

CDevice::~CDevice(void)
{
	Release();
}
