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
	//���ο� �����尡 ������ �ִ� ����.

	//���ҽ������� �ε��ϰ� �ȴ�.
	CLogo* pLogo = (CLogo*)pArg;

	EnterCriticalSection(&pLogo->GetCrt());

	/*CObjMgr::GetInstance()->LoadItemData();

	if(FAILED( CTextureMgr::GetInstance()->LoadImagePath(L"../Data/ImagePath.txt") ))
	{
	ERROR_MSG(L"��Ƽ�ؽ��� �̹��� �ε� ����!");
	return E_FAIL;
	}*/

	while(true)
	{
		if( GetAsyncKeyState('8') )
		{
			break;
		}
	}

	CTextureMgr::GetInstance()->m_wstrCurrentPath = L"�̹��� �ε� �Ϸ�!";

	LeaveCriticalSection(&pLogo->GetCrt());

	//�ٽ� ���� : �޸� �� ���ǻ����־��.
	//_endthreadex(0);

	return 0;
}

unsigned int CLogo::ImageLoadThreadFunc2( void * pArg )
{
	//���ο� �����尡 ������ �ִ� ����.

	//���ҽ������� �ε��ϰ� �ȴ�.
	CLogo* pLogo = (CLogo*)pArg;

	EnterCriticalSection(&pLogo->GetCrt());

	/*CObjMgr::GetInstance()->LoadItemData();

	if(FAILED( CTextureMgr::GetInstance()->LoadImagePath(L"../Data/ImagePath.txt") ))
	{
	ERROR_MSG(L"��Ƽ�ؽ��� �̹��� �ε� ����!");
	return E_FAIL;
	}*/

	while(true)
	{
		if( GetAsyncKeyState('8') )
		{
			break;
		}
	}

	CTextureMgr::GetInstance()->m_wstrCurrentPath = L"�̹��� �ε� �Ϸ�!";

	LeaveCriticalSection(&pLogo->GetCrt());

	//�ٽ� ���� : �޸� �� ���ǻ����־��.
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

	CTextureMgr::GetInstance()->m_wstrCurrentPath = L"�̹��� �ε� �Ϸ�!";

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

	//���� ����Ǵ���.. ������������ ����� ������ �Ǿ���Ѵ�.
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

