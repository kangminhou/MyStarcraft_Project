#include "stdafx.h"
#include "Starcraft_LoadingScene.h"

#include "TimeMgr.h"
#include "TextureMgr.h"
#include "Device.h"
#include "SceneMgr.h"
#include "KeyMgr.h"
#include "SoundMgr.h"


CStarcraft_LoadingScene::CStarcraft_LoadingScene()
	: m_bLoadingEnd( false )
{
}


CStarcraft_LoadingScene::~CStarcraft_LoadingScene()
{
}

#ifdef _IMAGE_THREAD_LOADING
CRITICAL_SECTION CStarcraft_LoadingScene::GetCrt( void )
{
	return m_Crt;
}

void CStarcraft_LoadingScene::LoadingEnd()
{
	this->m_bLoadingEnd = true;
}

unsigned int __stdcall CStarcraft_LoadingScene::ImageLoadThreadFunc(void* pArg)
{
	//새로운 스레드가 읽을수 있는 영역.

	//리소스정보를 로드하게 된다.
	CStarcraft_LoadingScene* pScene = (CStarcraft_LoadingScene*)pArg;

	EnterCriticalSection(&pScene->GetCrt());

	if ( !pScene->InitResource() )
	{
		ERROR_MSG( L"Texture Loading Failed ( Thread )" );
		return E_FAIL;
	}

	//if(FAILED( CTextureMgr::GetInstance()->LoadImagePath(L"../Data/ImagePath.txt") ))
	//{
	//	ERROR_MSG( L"멀티텍스쳐 이미지 로드 실패!" );
	//	return E_FAIL;
	//}

	CTextureMgr::GetInstance()->m_wstrCurrentPath = L"이미지 로드 완료!";

	LeaveCriticalSection(&pScene->GetCrt());

	pScene->LoadingEnd();

	//다시 설명 : 메모리 릭 주의사항있어요.
	//_endthreadex(0);


	return 0;
}

bool CStarcraft_LoadingScene::InitResource( void )
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

HRESULT CStarcraft_LoadingScene::Initialize( void )
{
	CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Single/MainLogo.png", L"LoadingBackground", TEX_SINGLE );
	m_pBackTexture = CTextureMgr::GetInstance()->GetTexture( L"LoadingBackground" );

	this->m_fWaitTime = 2.f;

	this->m_pSprite = CDevice::GetInstance()->GetSprite();
	this->m_pFont = CDevice::GetInstance()->GetFont();
	this->m_pTimeMgr = CTimeMgr::GetInstance();
	this->m_pKeyMgr = CKeyMgr::GetInstance();

	CSoundMgr::GetInstance()->PlayBGM( L"title.wav", CSoundMgr::Channel_BGM );

#ifdef _IMAGE_THREAD_LOADING
	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(
	NULL, 
	0,
	ImageLoadThreadFunc,
	this,
	NULL,
	NULL);
#else
	m_bLoadingEnd = true;
#endif

	return S_OK;
}

int CStarcraft_LoadingScene::Update( void )
{
	if ( m_bLoadingEnd )
	{
		this->m_fWaitTime -= this->m_pTimeMgr->GetTime();

		if ( this->m_fWaitTime <= 0.f || this->m_pKeyMgr->GetKeyOnceDown( VK_SPACE ) )
		{
			CSceneMgr::GetInstance()->SetChangeScene( SCENE_START );
			return 0;
		}
	}

	return 0;
}

void CStarcraft_LoadingScene::Render( void )
{
	D3DXMATRIX matTrans;
	D3DXMatrixTranslation( &matTrans, 0.f, 0.f, 0.f );
	this->m_pSprite->SetTransform( &matTrans );
	this->m_pSprite->Draw( this->m_pBackTexture->pTexture, nullptr, nullptr, nullptr, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

#ifdef _IMAGE_THREAD_LOADING
	D3DXMatrixTranslation(&matTrans, 50, 550, 0);

	this->m_pSprite->SetTransform(&matTrans);

	this->m_pFont->DrawTextW(
		this->m_pSprite,
		CTextureMgr::GetInstance()->m_wstrCurrentPath.c_str(),
		lstrlen(CTextureMgr::GetInstance()->m_wstrCurrentPath.c_str()),
		NULL,
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255)
	);
#endif

}

void CStarcraft_LoadingScene::Release( void )
{
}
