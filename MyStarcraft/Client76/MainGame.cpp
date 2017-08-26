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

	//CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Single/a.png", L"Test", TEX_SINGLE );

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

	//const TEX_INFO* pTex = CTextureMgr::GetInstance()->GetTexture( L"Test" );
	//static float fY = 0.f;
	//if ( GetAsyncKeyState( 'Z' ) )
	//{
	//	fY -= 0.01f;
	//}
	//else if ( GetAsyncKeyState( 'C' ) )
	//{
	//	fY += 0.01f;
	//}
	//RECT rc = { 0, fY, 56, 56 };

	//D3DXMATRIX matWorld;
	//D3DXMatrixTranslation( &matWorld, 100.f, 100.f + fY, 0.f );

	//CDevice::GetInstance()->GetSprite()->SetTransform( &matWorld );

	//CDevice::GetInstance()->GetSprite()->Draw(
	//	pTex->pTexture,
	//	&rc,
	//	&D3DXVECTOR3(pTex->ImageInfo.Width * 0.5f, pTex->ImageInfo.Height * 0.5f, 0.f),
	//	//NULL,
	//	NULL,
	//	D3DCOLOR_ARGB(255,255,255,255)
	//);

	//const TEX_INFO* pTexture = CTextureMgr::GetInstance()->GetTexture( L"BuildingOrder" );
	//
	//D3DXMATRIX matTrans;
	//D3DXMatrixTranslation( &matTrans, 300.f, 500.f, 0.f );
	//
	//CDevice::GetInstance()->GetSprite()->SetTransform( &matTrans );
	//CDevice::GetInstance()->GetSprite()->Draw( pTexture->pTexture, nullptr, nullptr, nullptr, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

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

	m_pDevice->DestroyInstance();
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

	///* Marine Image.. */
	//if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Unit/Marine/Idle/marine%d.png", L"Marine", TEX_MULTI, L"Idle", 17, true, D3DCOLOR_ARGB( 255, 0, 0, 0 ) ) ) )
	//	return false;
	//
	//if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Unit/Marine/Work/marine%d.png", L"Marine", TEX_MULTI, L"Move", 153, true, D3DCOLOR_ARGB( 255, 0, 0, 0 ) ) ) )
	//	return false;
	//
	//if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Unit/Marine/Attack/marine%d.png", L"Marine", TEX_MULTI, L"Attack", 51, true, D3DCOLOR_ARGB( 255, 0, 0, 0 ) ) ) )
	//	return false;
	//
	//if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Unit/Marine/Die/marine%d.png", L"Marine", TEX_MULTI, L"Die", 8, true, D3DCOLOR_ARGB( 255, 0, 0, 0 ) ) ) )
	//	return false;
	//
	///* Medic Image.. */
	//if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Unit/Medic/Idle/medic%d.png", L"Medic", TEX_MULTI, L"Idle", 17, true, D3DCOLOR_ARGB( 255, 0, 0, 0 ) ) ) )
	//	return false;
	//
	//if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Unit/Medic/Work/medic%d.png", L"Medic", TEX_MULTI, L"Move", 119, true, D3DCOLOR_ARGB( 255, 0, 0, 0 ) ) ) )
	//	return false;
	//
	//if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Unit/Medic/Attack/medic%d.png", L"Medic", TEX_MULTI, L"Attack", 34, true, D3DCOLOR_ARGB( 255, 0, 0, 0 ) ) ) )
	//	return false;
	//
	//if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Unit/Medic/Magic/medic%d.png", L"Medic", TEX_MULTI, L"Magic", 51, true, D3DCOLOR_ARGB( 255, 0, 0, 0 ) ) ) )
	//	return false;
	//
	//if ( FAILED( CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Unit/Medic/Die/medic%d.png", L"Medic", TEX_MULTI, L"Die", 9, true, D3DCOLOR_ARGB( 255, 0, 0, 0 ) ) ) )
	//	return false;

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
