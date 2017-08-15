#include "StdAfx.h"
#include "Stage.h"
#include "TextureMgr.h"
#include "objmgr.h"
#include "Factory.h"

#include "Marine.h"
#include "TestUnit.h"
#include "Background.h"
#include "Player.h"
#include "Medic.h"

#include "Include.h"
#include "Random.h"
#include "Device.h"

HRESULT CStage::Initialize( void )
{
	m_weaponManager.Initialize();
	CUnit::SetWeaponManager( &m_weaponManager );

	CObjMgr::GetInstance()->Initialize();

	CObjMgr::GetInstance()->AddGameObject( CFactory<CBackground>::CreateGameObject(), OBJ_TYPE_BACKGROUND );
	CObjMgr::GetInstance()->AddGameObject( CFactory<CPlayer>::CreateGameObject(), OBJ_TYPE_USER );

	CGameEntity::SetBackground( CObjMgr::GetInstance()->FindGameObject<CBackground>() );

	nCnt = 0;

	//for ( int i = 0; i < 50; ++i )
	//{
	//	++nCnt;
	//	CObjMgr::GetInstance()->AddGameObject( CFactory<CMarine>::CreateGameObject(RANDOM_RANGE_INTERGER(0, 600), RANDOM_RANGE_INTERGER(0, 200)), OBJ_TYPE_USER );
	//}


	//CObjMgr::GetInstance()->AddGameObject(CFactory<CMarine>::CreateGameObject(100, 100), OBJ_TYPE_USER);
	//CObjMgr::GetInstance()->AddGameObject(CFactory<CMarine>::CreateGameObject(200, 200), OBJ_TYPE_USER);
	//CObjMgr::GetInstance()->AddGameObject(CFactory<CMedic>::CreateGameObject(100, 200), OBJ_TYPE_USER);

	for ( int i = 0; i < 12; ++i )
		CObjMgr::GetInstance()->AddGameObject( CFactory<CMarine>::CreateGameObject( 100 + (i % 4) * 20, 100 + (i / 4) * 30 ), OBJ_TYPE_USER );

	for ( int i = 0; i < 12; ++i )
		CObjMgr::GetInstance()->AddGameObject( CFactory<CMarine>::CreateGameObject( 400 + (i % 4) * 20, 100 + (i / 4) * 30 ), OBJ_TYPE_USER2 );

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

	D3DXMATRIX matTrans;
	D3DXMatrixTranslation( &matTrans, 700.f, 100.f, 0.f );
	CDevice::GetInstance()->GetSprite()->SetTransform( &matTrans );
	CDevice::GetInstance()->GetFont()->DrawTextW(
		CDevice::GetInstance()->GetSprite(),
		str,
		lstrlen( str ),
		NULL,
		NULL,
		D3DCOLOR_ARGB( 255, 255, 255, 255 )
	);
}

void CStage::Release(void)
{
	CObjMgr::GetInstance()->Release();
}

CStage::CStage(void)
{
}

CStage::~CStage(void)
{
	Release();
}
