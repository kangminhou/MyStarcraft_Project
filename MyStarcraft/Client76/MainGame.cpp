#include "StdAfx.h"
#include "MainGame.h"

#include "Device.h"
#include "scenemgr.h"
#include "TextureMgr.h"
#include "TimeMgr.h"
#include "KeyMgr.h"
#include "Mouse.h"
#include "ObjMgr.h"
#include "Random.h"

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
		D3DCOLOR_ARGB(255, 255, 0, 0)
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
	{
		ERROR_MSG( L"Image Load Failed" );
		return E_FAIL;
	}
	
	CMouse::GetInstance()->Initialize();
	CRandom::GetInstance()->Initialize();

	CSceneMgr::GetInstance()->Initialize();
	CSceneMgr::GetInstance()->SetChangeScene(SCENE_STAGE);

	return S_OK;
}

int CMainGame::Update(void)
{
	CKeyMgr::GetInstance()->Update();
	CTimeMgr::GetInstance()->SetTime();
	CMouse::GetInstance()->Update();
	CSceneMgr::GetInstance()->Update();

	return 0;
}

void CMainGame::Render(void)
{
	m_pDevice->Render_Begin();

	CSceneMgr::GetInstance()->Render();

	CMouse::GetInstance()->Render();

	FPS_Render();
	
	m_pDevice->Render_End();
}

void CMainGame::Release(void)
{
	m_pDevice->DestroyInstance();
	CMouse::GetInstance()->DestroyInstance();
	CKeyMgr::GetInstance()->DestroyInstance();
	CSceneMgr::GetInstance()->DestroyInstance();
	CTextureMgr::GetInstance()->DestroyInstance();
	CObjMgr::GetInstance()->DestroyInstance();
	CTimeMgr::GetInstance()->DestroyInstance();
	CRandom::GetInstance()->DestroyInstance();
}

bool CMainGame::InitResource( void )
{
	/* UI Image.. */
	if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/UI/DragArea.png", L"Drag", TEX_SINGLE, L"", 1, true, D3DCOLOR_ARGB(255,0,0,0) ) ) )
		return false;

	if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/UI/PSelect/PSelect%d.png", L"SelectArea", TEX_MULTI, L"Player", 10, true, D3DCOLOR_ARGB( 255, 255, 255, 255 ) ) ) )
		return false;

	if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/UI/ESelect/ESelect%d.png", L"SelectArea", TEX_MULTI, L"Enemy", 10, true, D3DCOLOR_ARGB( 255, 255, 255, 255 ) ) ) )
		return false;

	/* Map Image.. */
	if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Tile/DebugTile%d.png"
		 , L"Back"
		 , TEX_MULTI
		 , L"Tile"
		 , 16
		 , true
		 , D3DCOLOR_ARGB( 255, 255, 255, 255 ) ) ) )
		return false;

	if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Map/Map.png", L"BackGround", TEX_SINGLE ) ) )
		return false;

	if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Map/TestMap.png", L"Map", TEX_SINGLE ) ) )
		return false;

	/* Marine Image.. */
	if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Unit/Marine/Idle/marine%d.png", L"Marine", TEX_MULTI, L"Idle", 17, true, D3DCOLOR_ARGB( 255, 0, 0, 0 ) ) ) )
		return false;

	if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Unit/Marine/Work/marine%d.png", L"Marine", TEX_MULTI, L"Move", 153, true, D3DCOLOR_ARGB( 255, 0, 0, 0 ) ) ) )
		return false;

	if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Unit/Marine/Attack/marine%d.png", L"Marine", TEX_MULTI, L"Attack", 51, true, D3DCOLOR_ARGB( 255, 0, 0, 0 ) ) ) )
		return false;

	if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Unit/Marine/Die/marine%d.png", L"Marine", TEX_MULTI, L"Die", 8, true, D3DCOLOR_ARGB( 255, 0, 0, 0 ) ) ) )
		return false;

	/* Medic Image.. */
	if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Unit/Medic/Idle/medic%d.png", L"Medic", TEX_MULTI, L"Idle", 17, true, D3DCOLOR_ARGB( 255, 0, 0, 0 ) ) ) )
		return false;

	if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Unit/Medic/Work/medic%d.png", L"Medic", TEX_MULTI, L"Move", 119, true, D3DCOLOR_ARGB( 255, 0, 0, 0 ) ) ) )
		return false;

	if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Unit/Medic/Attack/medic%d.png", L"Medic", TEX_MULTI, L"Attack", 34, true, D3DCOLOR_ARGB( 255, 0, 0, 0 ) ) ) )
		return false;

	if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Unit/Medic/Magic/medic%d.png", L"Medic", TEX_MULTI, L"Magic", 51, true, D3DCOLOR_ARGB( 255, 0, 0, 0 ) ) ) )
		return false;

	if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Unit/Medic/Die/medic%d.png", L"Medic", TEX_MULTI, L"Die", 9, true, D3DCOLOR_ARGB( 255, 0, 0, 0 ) ) ) )
		return false;

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
