#include "StdAfx.h"
#include "Stage.h"
#include "TextureMgr.h"
#include "objmgr.h"
#include "Factory.h"
#include "EffectMgr.h"

#include "Marine.h"
#include "TestUnit.h"
#include "Background.h"
#include "Player.h"
#include "Medic.h"
#include "Firebat.h"
#include "Vulture.h"
#include "Tank.h"
#include "Goliath.h"

#include "Include.h"
#include "Random.h"
#include "Device.h"

HRESULT CStage::Initialize( void )
{
	m_weaponManager.Initialize();
	CUnit::SetWeaponManager( &m_weaponManager );

	CObjMgr::GetInstance()->Initialize();
	CEffectMgr::GetInstance()->Initialize();

	CObjMgr::GetInstance()->AddGameObject( CFactory<CBackground>::CreateGameObject(), OBJ_TYPE_BACKGROUND );
	CObjMgr::GetInstance()->AddGameObject( CFactory<CPlayer>::CreateGameObject(), OBJ_TYPE_USER );

	CGameEntity::SetBackground( CObjMgr::GetInstance()->FindGameObject<CBackground>() );


	//CObjMgr::GetInstance()->AddGameObject(CFactory<CMarine>::CreateGameObject(100, 100), OBJ_TYPE_USER);
	//CObjMgr::GetInstance()->AddGameObject(CFactory<CMarine>::CreateGameObject(200, 200), OBJ_TYPE_USER);
	//CObjMgr::GetInstance()->AddGameObject(CFactory<CMedic>::CreateGameObject(100, 200), OBJ_TYPE_USER);
	CObjMgr::GetInstance()->AddGameObject(CFactory<CMarine>::CreateGameObject(500, 200), OBJ_TYPE_USER2);

	//CObjMgr::GetInstance()->AddGameObject( CFactory<CFirebat>::CreateGameObject( 100, 400 ), OBJ_TYPE_USER );

	//CObjMgr::GetInstance()->AddGameObject( CFactory<CVulture>::CreateGameObject( 100, 400 ), OBJ_TYPE_USER );
	CObjMgr::GetInstance()->AddGameObject( CFactory<CTank>::CreateGameObject( 100, 400 ), OBJ_TYPE_USER );
	//CObjMgr::GetInstance()->AddGameObject( CFactory<CGoliath>::CreateGameObject( 100, 400 ), OBJ_TYPE_USER );

	//for ( int i = 0; i < 12; ++i )
	//	CObjMgr::GetInstance()->AddGameObject( CFactory<CMarine>::CreateGameObject( 100 + (i % 4) * 20, 100 + (i / 4) * 30 ), OBJ_TYPE_USER );
	
	//for ( int i = 0; i < 12; ++i )
	//	CObjMgr::GetInstance()->AddGameObject( CFactory<CMarine>::CreateGameObject( 400 + (i % 4) * 20, 100 + (i / 4) * 30 ), OBJ_TYPE_USER2 );

	CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Effect/SiegeTankFire/STankFire%d.png", L"Tank", TEX_MULTI, L"Fire", 119, true, D3DCOLOR_ARGB( 255, 0, 255, 0 ) );

	return S_OK;
}

int CStage::Update(void)
{
	CObjMgr::GetInstance()->Update();

	return 0;
}

void CStage::Render(void)
{
	CObjMgr::GetInstance()->Render();

	TCHAR str[128] = L"";
	swprintf_s( str, L"%d", nCnt );

	//D3DXMATRIX matTrans;
	//D3DXMatrixTranslation( &matTrans, 700.f, 100.f, 0.f );
	//CDevice::GetInstance()->GetSprite()->SetTransform( &matTrans );
	//CDevice::GetInstance()->GetFont()->DrawTextW(
	//	CDevice::GetInstance()->GetSprite(),
	//	str,
	//	lstrlen( str ),
	//	NULL,
	//	NULL,
	//	D3DCOLOR_ARGB( 255, 255, 255, 255 )
	//);

	static float cnt = 0;
	cnt += 0.02f;
	if ( cnt > 6.f )
		cnt = 0.f;
	const TEX_INFO* pTex = CTextureMgr::GetInstance()->GetTexture( L"Tank", L"Fire", (int)cnt );

	D3DXMATRIX matTrans;
	D3DXMatrixTranslation( &matTrans, 500.f, 100.f, 0.f );
	CDevice::GetInstance()->GetSprite()->SetTransform( &matTrans );
	CDevice::GetInstance()->GetSprite()->Draw(
		pTex->pTexture,
		NULL,
		&D3DXVECTOR3( pTex->ImageInfo.Width * 0.5f, pTex->ImageInfo.Height * 0.5f, 0.f ),
		NULL,
		D3DCOLOR_ARGB( 255, 255, 255, 255 )
	);
}

void CStage::Release(void)
{
	CObjMgr::GetInstance()->Release();
	CEffectMgr::GetInstance()->DestroyInstance();
}

CStage::CStage(void)
{
}

CStage::~CStage(void)
{
	Release();
}
