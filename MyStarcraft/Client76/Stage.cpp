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
#include "Mineral.h"
#include "Gas.h"
#include "ResourceMgr.h"
#include "EnemyMgr.h"
#include "AStarManager.h"
#include "ResearchMgr.h"

#include "Include.h"
#include "Random.h"
#include "Device.h"
#include "UIMgr.h"
#include "UpgradeMgr.h"

HRESULT CStage::Initialize( void )
{
	m_weaponManager.Initialize();
	CUnit::SetWeaponManager( &m_weaponManager );
	CBuilding::InitBasicBuildTexture();

	CObjMgr::GetInstance()->Initialize();
	CEffectMgr::GetInstance()->Initialize();

	CUIMgr::GetInstance()->Initialize();
	CEntityMgr::GetInstance()->Initialize();
	CResourceMgr::GetInstance()->Initialize();
	CUpgradeMgr::GetInstance()->Initialize();
	CUpgradeMgr::GetInstance()->SetWeaponMgr( &m_weaponManager );

	CResearchMgr::GetInstance()->Initialize();

	CObjMgr::GetInstance()->AddGameObject( CFactory<CBackground>::CreateGameObject(), OBJ_TYPE_BACKGROUND );

	/* Resource Setting.. */
	CResourceMgr::GetInstance()->MakeResource( D3DXVECTOR3( 186.f, 2560.f, 0.f ), CResourceMgr::Resource_Mineral, OBJ_TYPE_RESOURCE );
	CResourceMgr::GetInstance()->MakeResource( D3DXVECTOR3( 186.f, 2592.f, 0.f ), CResourceMgr::Resource_Mineral, OBJ_TYPE_RESOURCE );
	CResourceMgr::GetInstance()->MakeResource( D3DXVECTOR3( 186.f, 2630.f, 0.f ), CResourceMgr::Resource_Mineral, OBJ_TYPE_RESOURCE );
	CResourceMgr::GetInstance()->MakeResource( D3DXVECTOR3( 186.f, 2670.f, 0.f ), CResourceMgr::Resource_Mineral, OBJ_TYPE_RESOURCE );
	CResourceMgr::GetInstance()->MakeResource( D3DXVECTOR3( 186.f, 2702.f, 0.f ), CResourceMgr::Resource_Mineral, OBJ_TYPE_RESOURCE );
	CResourceMgr::GetInstance()->MakeResource( D3DXVECTOR3( 186.f, 2732.f, 0.f ), CResourceMgr::Resource_Mineral, OBJ_TYPE_RESOURCE );
	CResourceMgr::GetInstance()->MakeResource( D3DXVECTOR3( 186.f, 2770.f, 0.f ), CResourceMgr::Resource_Mineral, OBJ_TYPE_RESOURCE );
	CResourceMgr::GetInstance()->MakeResource( D3DXVECTOR3( 186.f, 2806.f, 0.f ), CResourceMgr::Resource_Mineral, OBJ_TYPE_RESOURCE );

	CObjMgr::GetInstance()->AddGameObject( CFactory<CPlayer>::CreateGameObject(), OBJ_TYPE_USER );

	CGameEntity::SetBackground( CObjMgr::GetInstance()->FindGameObject<CBackground>() );

	CEnemyMgr::GetInstance()->Initialize();

	/* Make Command Center.. */
	CGameObject* pObj = CEntityMgr::GetInstance()->MakeUnit(
		CEntityMgr::Entity_Control, D3DXVECTOR3( 368.f, 2672.f, 0.f ), OBJ_TYPE_USER, true );

	CObjMgr::GetInstance()->AddGameObject( pObj, pObj->GetObjectType() );

	return S_OK;
}

#include "Mouse.h"
#include "KeyMgr.h"
int CStage::Update(void)
{
	if ( CKeyMgr::GetInstance()->GetKeyOnceDown( VK_LBUTTON ) )
	{
		if ( CKeyMgr::GetInstance()->GetKeyStayDown( 'Q' ) )
		{
			D3DXVECTOR3 vMousePos = CMouse::GetInstance()->GetPos() + CGameObject::GetScroll();
			CResourceMgr::GetInstance()->MakeResource( vMousePos, CResourceMgr::Resource_Mineral, OBJ_TYPE_RESOURCE );
			cout << "vMousePos.x : " << vMousePos.x << ", vMousePos.y : " << vMousePos.y << endl;
		}
		else if ( CKeyMgr::GetInstance()->GetKeyStayDown( 'G' ) )
		{
			D3DXVECTOR3 vMousePos = CMouse::GetInstance()->GetPos() + CGameObject::GetScroll();
			CResourceMgr::GetInstance()->MakeResource( vMousePos, CResourceMgr::Resource_Gas, OBJ_TYPE_RESOURCE );
			cout << "vMousePos.x : " << vMousePos.x << ", vMousePos.y : " << vMousePos.y << endl;
		}
	}

	CAStarManager::GetInstance()->Update();
	CEnemyMgr::GetInstance()->Update();
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
	CResourceMgr::GetInstance()->DestroyInstance();
	CObjMgr::GetInstance()->Release();
	CEffectMgr::GetInstance()->DestroyInstance();
	CUIMgr::GetInstance()->DestroyInstance();
	CEntityMgr::GetInstance()->DestroyInstance();
	CUpgradeMgr::GetInstance()->DestroyInstance();
	CAStarManager::GetInstance()->DestroyInstance();
	CEnemyMgr::GetInstance()->DestroyInstance();
	CResearchMgr::GetInstance()->DestroyInstance();
}

CStage::CStage(void)
{
}

CStage::~CStage(void)
{
	Release();
}
