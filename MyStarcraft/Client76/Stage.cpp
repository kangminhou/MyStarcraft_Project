#include "StdAfx.h"
#include "Stage.h"
#include "TextureMgr.h"
#include "objmgr.h"
#include "Factory.h"
#include "EffectMgr.h"

#include "Background.h"
#include "Player.h"
#include "Building.h"
#include "Unit.h"
#include "EntityMgr.h"

#include "Include.h"
#include "Random.h"
#include "Device.h"
#include "UIMgr.h"

HRESULT CStage::Initialize( void )
{
	m_weaponManager.Initialize();
	CUnit::SetWeaponManager( &m_weaponManager );
	CBuilding::InitBasicBuildTexture();

	CObjMgr::GetInstance()->Initialize();
	CEffectMgr::GetInstance()->Initialize();

	CUIMgr::GetInstance()->Initialize();
	CEntityMgr::GetInstance()->Initialize();

	CObjMgr::GetInstance()->AddGameObject( CFactory<CBackground>::CreateGameObject(), OBJ_TYPE_BACKGROUND );
	CObjMgr::GetInstance()->AddGameObject( CFactory<CPlayer>::CreateGameObject(), OBJ_TYPE_USER );

	CGameEntity::SetBackground( CObjMgr::GetInstance()->FindGameObject<CBackground>() );

	CGameObject* pObj = CEntityMgr::GetInstance()->MakeUnit(
		CEntityMgr::Entity_SCV, D3DXVECTOR3(300.f, 100.f, 0.f), OBJ_TYPE_USER );
	
	CObjMgr::GetInstance()->AddGameObject( pObj, pObj->GetObjectType() );

	//CGameObject* pObj = CEntityMgr::GetInstance()->MakeUnit( 
	//	CEntityMgr::Entity_Control, D3DXVECTOR3(300.f, 100.f, 0.f), OBJ_TYPE_USER );
	//
	//CObjMgr::GetInstance()->AddGameObject( pObj, pObj->GetObjectType() );

	//for ( int i = 0; i < 12; ++i )
	//{
	//	CGameObject* pObj = CEntityMgr::GetInstance()->MakeUnit(
	//		CEntityMgr::Entity_Marine, D3DXVECTOR3(100 + (i % 10) * 20, 100 + (i / 10) * 30, 0.f), OBJ_TYPE_USER );
	//
	//	CObjMgr::GetInstance()->AddGameObject( pObj, pObj->GetObjectType() );
	//}
	//
	//for ( int i = 0; i < 2; ++i )
	//{
	//	CGameObject* pObj = CEntityMgr::GetInstance()->MakeUnit(
	//		CEntityMgr::Entity_Medic, D3DXVECTOR3(100 + (i % 10) * 20, 400 + (i / 10) * 30, 0.f), OBJ_TYPE_USER );
	//
	//	CObjMgr::GetInstance()->AddGameObject( pObj, pObj->GetObjectType() );
	//}

	//for ( int i = 0; i < 50; ++i )
	//{
	//	CGameObject* pObj = CEntityMgr::GetInstance()->MakeUnit( 
	//		CEntityMgr::Entity_Medic, D3DXVECTOR3(300 + (i % 10) * 20, 100 + (i / 10) * 30, 0.f), OBJ_TYPE_USER );
	//
	//	CObjMgr::GetInstance()->AddGameObject( pObj, pObj->GetObjectType() );
	//}

	for ( int i = 0; i < 12; ++i )
	{
		CGameObject* pObj = CEntityMgr::GetInstance()->MakeUnit( 
			CEntityMgr::Entity_Marine, D3DXVECTOR3(1500 + (i % 10) * 20, 100 + (i / 10) * 30, 0.f), OBJ_TYPE_USER2 );
	
		CObjMgr::GetInstance()->AddGameObject( pObj, pObj->GetObjectType() );
	}

	//CObjMgr::GetInstance()->AddGameObject(
	//	CEntityMgr::GetInstance()->MakeUnit( CEntityMgr::Entity_SCV, D3DXVECTOR3( 400, 400, 0.f ), OBJ_TYPE_USER ), OBJ_TYPE_USER
	//);

	CTextureMgr::GetInstance()->InsertTexture( L"../Texture/Effect/SiegeTankFire/STankFire%d.png", L"Tank", TEX_MULTI, L"Fire", 119, true, D3DCOLOR_ARGB( 255, 0, 255, 0 ) );

	return S_OK;
}

int CStage::Update(void)
{
	//if ( CKeyMgr::GetInstance()->GetKeyOnceDown( VK_LBUTTON ) )
	//{
	//	if ( CKeyMgr::GetInstance()->GetKeyStayDown( 'Q' ) )
	//	{
	//		D3DXVECTOR3 vMousePos = CMouse::GetInstance()->GetPos() + CGameObject::GetScroll();
	//		CObjMgr::GetInstance()->AddGameObject( CFactory<CMarine>::CreateGameObject( vMousePos.x, vMousePos.y ), OBJ_TYPE_USER );
	//	}
	//	else 
	//	{
	//		D3DXVECTOR3 vMousePos = CMouse::GetInstance()->GetPos() + CGameObject::GetScroll();
	//		CObjMgr::GetInstance()->AddGameObject( CFactory<CMarine>::CreateGameObject( vMousePos.x, vMousePos.y ), OBJ_TYPE_USER2 );
	//	}
	//}

	CObjMgr::GetInstance()->Update();
	CUIMgr::GetInstance()->Update();

	return 0;
}

void CStage::Render(void)
{
	CObjMgr::GetInstance()->Render();

	CUIMgr::GetInstance()->Render();

	//TCHAR str[128] = L"";
	//swprintf_s( str, L"%d", nCnt );
	//
	////D3DXMATRIX matTrans;
	////D3DXMatrixTranslation( &matTrans, 700.f, 100.f, 0.f );
	////CDevice::GetInstance()->GetSprite()->SetTransform( &matTrans );
	////CDevice::GetInstance()->GetFont()->DrawTextW(
	////	CDevice::GetInstance()->GetSprite(),
	////	str,
	////	lstrlen( str ),
	////	NULL,
	////	NULL,
	////	D3DCOLOR_ARGB( 255, 255, 255, 255 )
	////);
	//
	//static float cnt = 0;
	//cnt += 0.02f;
	//if ( cnt > 6.f )
	//	cnt = 0.f;
	//const TEX_INFO* pTex = CTextureMgr::GetInstance()->GetTexture( L"Tank", L"Fire", (int)cnt );
	//
	//D3DXMATRIX matTrans;
	//D3DXMatrixTranslation( &matTrans, 500.f, 100.f, 0.f );
	//CDevice::GetInstance()->GetSprite()->SetTransform( &matTrans );
	//CDevice::GetInstance()->GetSprite()->Draw(
	//	pTex->pTexture,
	//	NULL,
	//	&D3DXVECTOR3( pTex->ImageInfo.Width * 0.5f, pTex->ImageInfo.Height * 0.5f, 0.f ),
	//	NULL,
	//	D3DCOLOR_ARGB( 255, 255, 255, 255 )
	//);
}

void CStage::Release(void)
{
	CObjMgr::GetInstance()->Release();
	CEffectMgr::GetInstance()->DestroyInstance();
	CUIMgr::GetInstance()->DestroyInstance();
	CEntityMgr::GetInstance()->DestroyInstance();
}

CStage::CStage(void)
{
}

CStage::~CStage(void)
{
	Release();
}
