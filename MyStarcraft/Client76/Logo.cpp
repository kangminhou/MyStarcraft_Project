#include "StdAfx.h"
#include "Logo.h"
#include "SceneMgr.h"
#include "ObjMgr.h"

#include "TextureMgr.h"
#include "TimeMgr.h"
#include "Device.h"

#include <process.h>
#include <thread>

#include <future>
#include <chrono>

CLogo::CLogo(void)
{
}

CLogo::~CLogo(void)
{
	Release();
}

unsigned int CLogo::ImageLoadThreadFunc( void * pArg )
{
	//새로운 스레드가 읽을수 있는 영역.

	//리소스정보를 로드하게 된다.
	CLogo* pLogo = (CLogo*)pArg;

	EnterCriticalSection(&pLogo->GetCrt());

	/*CObjMgr::GetInstance()->LoadItemData();

	if(FAILED( CTextureMgr::GetInstance()->LoadImagePath(L"../Data/ImagePath.txt") ))
	{
	ERROR_MSG(L"멀티텍스쳐 이미지 로드 실패!");
	return E_FAIL;
	}*/

	while(true)
	{
		if( GetAsyncKeyState('8') )
		{
			break;
		}
	}

	CTextureMgr::GetInstance()->m_wstrCurrentPath = L"이미지 로드 완료!";

	LeaveCriticalSection(&pLogo->GetCrt());

	//다시 설명 : 메모리 릭 주의사항있어요.
	//_endthreadex(0);

	return 0;
}

unsigned int CLogo::ImageLoadThreadFunc2( void * pArg )
{
	//새로운 스레드가 읽을수 있는 영역.

	//리소스정보를 로드하게 된다.
	CLogo* pLogo = (CLogo*)pArg;

	EnterCriticalSection(&pLogo->GetCrt());

	/*CObjMgr::GetInstance()->LoadItemData();

	if(FAILED( CTextureMgr::GetInstance()->LoadImagePath(L"../Data/ImagePath.txt") ))
	{
	ERROR_MSG(L"멀티텍스쳐 이미지 로드 실패!");
	return E_FAIL;
	}*/

	while(true)
	{
		if( GetAsyncKeyState('8') )
		{
			break;
		}
	}

	CTextureMgr::GetInstance()->m_wstrCurrentPath = L"이미지 로드 완료!";

	LeaveCriticalSection(&pLogo->GetCrt());

	//다시 설명 : 메모리 릭 주의사항있어요.
	//_endthreadex(0);

	return 0;
}

unsigned int CLogo::TestFunc( void * pArg )
{
	//fTime1 += GET_TIME;

	while(true)
	{
		fTime1 += GET_TIME;

		if( GetAsyncKeyState('8') )
		{
			break;
		}
	}

	CTextureMgr::GetInstance()->m_wstrCurrentPath = L"이미지 로드 완료!";

	return 0;
}

HRESULT CLogo::Initialize( void )
{
	fTime1 = fTime2 = 0.f;

	return S_OK;
}

int CLogo::Update(void)
{
	if( GetAsyncKeyState(VK_LBUTTON) )
	{
//		CSceneMgr::GetInstance()->SetScene(SCENE_STAGE);
		CSceneMgr::GetInstance()->SetChangeScene(SCENE_STAGE);
	}

	//씬이 변경되더라도.. 씬영역에서의 기능은 마무리 되어야한다.
	fTime2 += GET_TIME;

	return 0;
}

void CLogo::Render(void)
{
	D3DXMATRIX matTrans;

	D3DXMatrixTranslation(&matTrans, 50, 550, 0);

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

	CDevice::GetInstance()->GetFont()->DrawTextW(
		CDevice::GetInstance()->GetSprite(),
		CTextureMgr::GetInstance()->m_wstrCurrentPath.c_str(),
		lstrlen(CTextureMgr::GetInstance()->m_wstrCurrentPath.c_str()),
		NULL,
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255)
	);

	TCHAR str[128] = L"";
	swprintf_s( str, L"%f, %f", fTime1, fTime2 );

	CDevice::GetInstance()->GetFont()->DrawTextW(
		CDevice::GetInstance()->GetSprite(),
		str,
		lstrlen(str),
		NULL,
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255)
	);
}

void CLogo::Release(void)
{
	//CObjMgr::GetInstance()->Release();
	CObjMgr::GetInstance()->Release(OBJ_TYPE_USER);
}

