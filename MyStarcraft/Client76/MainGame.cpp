#include "StdAfx.h"
#include "MainGame.h"

#include "Device.h"
#include "scenemgr.h"
#include "TextureMgr.h"
#include "TimeMgr.h"
#include "KeyMgr.h"

void CMainGame::FPS_Render(void)
{
	D3DXMatrixTranslation(&m_matTrans, 30, 30, 0.f);
	
	++m_iCount;

	//m_fTime += CTimeMgr::GetInstance()->GetTime();

	m_fTime += GET_TIME;

	if(m_fTime > 1.f)
	{
		m_fTime = 0.f;

		wsprintf(m_szFps, L"FPS : %d", m_iCount);

		m_iCount = 0;
	}

	m_pDevice->GetSprite()->SetTransform(&m_matTrans);

	m_pDevice->GetFont()->DrawTextW(
		m_pDevice->GetSprite(),
		m_szFps,
		lstrlen(m_szFps),
		NULL,
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255)
		);
}

HRESULT CMainGame::Initialize(void)
{
	CTimeMgr::GetInstance()->InitTime();
	CKeyMgr::GetInstance()->Initialize();
	
	if(FAILED(m_pDevice->Init3D()))
	{
		ERROR_MSG(L"장치 초기화 실패!");
		return E_FAIL;
	}
	
	if ( !InitResource() )
		return E_FAIL;
	
	CSceneMgr::GetInstance()->Initialize();
	CSceneMgr::GetInstance()->SetChangeScene(SCENE_STAGE);

	return S_OK;
}

int CMainGame::Update(void)
{
	CKeyMgr::GetInstance()->Update();
	CTimeMgr::GetInstance()->SetTime();
	CSceneMgr::GetInstance()->Update();

	return 0;
}

void CMainGame::Render(void)
{
	m_pDevice->Render_Begin();

	CSceneMgr::GetInstance()->Render();

	FPS_Render();
	
	m_pDevice->Render_End();
}

void CMainGame::Release(void)
{
	m_pDevice->DestroyInstance();
	CKeyMgr::GetInstance()->DestroyInstance();
	CSceneMgr::GetInstance()->DestroyInstance();
	CTextureMgr::GetInstance()->DestroyInstance();
}

bool CMainGame::InitResource( void )
{
	if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Tile/Tile%d.png"
		 , L"Back"
		 , TEX_MULTI
		 , L"Tile"
		 , 2 ) ) )
	{
		ERROR_MSG( L"타일 이미지 로드 실패!" );
		return false;
	}

	if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Map/Map.png", L"BackGround", TEX_SINGLE ) ) )
	{
		ERROR_MSG( L"타일 이미지 로드 실패!" );
		return false;
	}

	return true;

}

CMainGame::CMainGame(void)
:m_pDevice(CDevice::GetInstance())
,m_fTime(0.f)
,m_iCount(0)
{
	ZeroMemory(m_szFps, sizeof(TCHAR) * MIN_STR);
	D3DXMatrixIdentity(&m_matTrans);
}

CMainGame::~CMainGame(void)
{
	Release();
}
