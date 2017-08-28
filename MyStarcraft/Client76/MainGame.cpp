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
#include "SoundMgr.h"

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
	this->m_pMouse = CMouse::GetInstance();
	this->m_pSceneMgr = CSceneMgr::GetInstance();
	this->m_pKeyMgr = CKeyMgr::GetInstance();
	this->m_pSoundMgr = CSoundMgr::GetInstance();
	this->m_pTimeMgr = CTimeMgr::GetInstance();

	this->m_pTimeMgr->InitTime();
	this->m_pKeyMgr->Initialize();
	this->m_pSoundMgr->Initialize();

	if(FAILED(m_pDevice->Init3D()))
	{
		ERROR_MSG(L"장치 초기화 실패!");
		return E_FAIL;
	}

#ifndef _IMAGE_THREAD_LOADING
	if ( !InitResource() )
	{
		ERROR_MSG( L"Image Load Failed" );
		return E_FAIL;
	}
#endif

	if ( FAILED( this->InitSound() ) )
	{
		ERROR_MSG( L"Sound Load Failed" );
		return E_FAIL;
	}

	this->m_pMouse->Initialize();
	CRandom::GetInstance()->Initialize();

	this->m_pSceneMgr->Initialize();
	this->m_pSceneMgr->SetChangeScene( SCENE_Loading );

	//CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Single/a.png", L"Test", TEX_SINGLE );

	return S_OK;
}

int CMainGame::Update(void)
{
	this->m_pSoundMgr->UpdateSound();
	this->m_pKeyMgr->Update();
	this->m_pTimeMgr->SetTime();
	this->m_pMouse->Update();
	this->m_pSceneMgr->Update();

	return 0;
}

void CMainGame::Render(void)
{
	m_pDevice->Render_Begin();

	this->m_pSceneMgr->Render();

	this->m_pMouse->Render();

	FPS_Render();
	
	m_pDevice->Render_End();
}

void CMainGame::Release(void)
{
	CMouse::GetInstance()->DestroyInstance();
	CKeyMgr::GetInstance()->DestroyInstance();
	CSceneMgr::GetInstance()->DestroyInstance();
	CTextureMgr::GetInstance()->DestroyInstance();
	CObjMgr::GetInstance()->DestroyInstance();
	CTimeMgr::GetInstance()->DestroyInstance();
	CRandom::GetInstance()->DestroyInstance();
	CSoundMgr::GetInstance()->DestroyInstance();

	m_pDevice->DestroyInstance();
}

#ifndef _IMAGE_THREAD_LOADING
bool CMainGame::InitResource( void )
{
	/* UI Image.. */
	if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/UI/DragArea.png", L"Drag", TEX_SINGLE, L"", 1, true, D3DCOLOR_ARGB(255,0,0,0) ) ) )
		return false;

	if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/UI/PSelect/PSelect%d.png", L"SelectArea", TEX_MULTI, L"Player", 10, true, D3DCOLOR_ARGB( 255, 255, 255, 255 ) ) ) )
		return false;

	if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/UI/ESelect/ESelect%d.png", L"SelectArea", TEX_MULTI, L"Enemy", 10, true, D3DCOLOR_ARGB( 255, 255, 255, 255 ) ) ) )
		return false;

	if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Single/MainUi.png", L"MainUi", TEX_SINGLE ) ) )
		return false;

	if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Single/UnitUpgrade_Back.png", L"UnitUpgradeBack", TEX_SINGLE ) ) )
		return false;

	if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Single/HpBar_Back%d.png", L"HpBar", TEX_MULTI, L"Back", 1 ) ) )
		return false;

	if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/UI/HpGauge/HpBar_Front%d.png", L"HpBar", TEX_MULTI, L"Front", 4 ) ) )
		return false;

	if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Minimap/Entity%d.png", L"Minimap", TEX_MULTI, L"Entity", 2 ) ) )
		return false;

	if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Single/BuildingOrder.png", L"BuildingOrder", TEX_SINGLE ) ) )
		return false;

	if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Single/ProgressBar.png", L"ProgressBar", TEX_SINGLE ) ) )
		return false;

	if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Single/ProgressGauge.png", L"ProgressGauge", TEX_SINGLE ) ) )
		return false;

	if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Tile/AStarTest/TestTile5.png", L"BlackTile", TEX_SINGLE ) ) )
		return false;

	/* Start Scene Use Image.. */
	if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Single/MenuBack.png", L"StartBack", TEX_SINGLE ) ) )
		return false;

	if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/MainMenu/Button/editor_button%d.png", L"BUTTON", TEX_MULTI, L"EDIT", 2 ) ) )
		return false;

	if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/MainMenu/Button/exit_button%d.png", L"BUTTON", TEX_MULTI, L"EXIT", 2 ) ) )
		return false;

	if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/MainMenu/Button/single_button%d.png", L"BUTTON", TEX_MULTI, L"SINGLE", 2 ) ) )
		return false;

	/* Player Resource Data Image.. */
	if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Single/Player_Info_Gas.png", L"Info_Gas", TEX_SINGLE ) ) )
		return false;

	if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Single/Player_Info_Mineral.png", L"Info_Mineral", TEX_SINGLE ) ) )
		return false;

	if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Single/Player_Info_Human.png", L"Info_Human", TEX_SINGLE ) ) )
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

	if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Map/MiniMapRect.png", L"Mini_Area", TEX_SINGLE ) ) )
		return false;

	if ( FAILED( CTextureMgr::GetInstance()->LoadImagePath( L"../Data/ImagePath.txt" ) ) )
		return false;

	/* Resuorces Image.. */
	if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Resources/Mineral/Mineral%d.png", L"Mineral", TEX_MULTI, L"Trunk", 12, true, D3DCOLOR_ARGB( 255, 0, 0, 0 ) ) ) )
		return false;

	if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Resources/Mineral/Shad/Mineral_Shad%d.png", L"Mineral", TEX_MULTI, L"Shad", 12, true, D3DCOLOR_ARGB( 255, 0, 0, 0 ) ) ) )
		return false;

	if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Resources/Gas/Gas%d.png", L"Gas", TEX_MULTI, L"Trunk", 1, true, D3DCOLOR_ARGB( 255, 0, 0, 0 ) ) ) )
		return false;

	if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Resources/Gas/Gas_Shad%d.png", L"Gas", TEX_MULTI, L"Shad", 4, true, D3DCOLOR_ARGB( 255, 0, 0, 0 ) ) ) )
		return false;

	if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Single/Build_OK.png", L"Build_OK", TEX_SINGLE ) ) )
		return false;

	if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Single/Build_NO.png", L"Build_OFF", TEX_SINGLE ) ) )
		return false;

	CTextureMgr::GetInstance()->LoadingEnd();

	return true;

}
#endif

HRESULT CMainGame::InitSound( void )
{
	this->m_pSoundMgr->LoadSoundFile( "../Sound/scv/" );
	this->m_pSoundMgr->LoadSoundFile( "../Sound/marine/" );
	this->m_pSoundMgr->LoadSoundFile( "../Sound/firebat/" );
	this->m_pSoundMgr->LoadSoundFile( "../Sound/vulture/" );
	this->m_pSoundMgr->LoadSoundFile( "../Sound/tank/" );
	this->m_pSoundMgr->LoadSoundFile( "../Sound/medic/" );
	this->m_pSoundMgr->LoadSoundFile( "../Sound/tank/" );
	this->m_pSoundMgr->LoadSoundFile( "../Sound/goliath/" );
	this->m_pSoundMgr->LoadSoundFile( "../Sound/ghost/" );
	this->m_pSoundMgr->LoadSoundFile( "../Sound/advisor/" );
	this->m_pSoundMgr->LoadSoundFile( "../Sound/BGM/" );

	return S_OK;
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
