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
#include "SoundMgr.h"

#include "Include.h"
#include "Random.h"
#include "Device.h"
#include "UIMgr.h"
#include "UpgradeMgr.h"

HRESULT CStage::Initialize( void )
{
	this->m_pAStarMgr = CAStarManager::GetInstance();
	this->m_pEnemyMgr = CEnemyMgr::GetInstance();
	this->m_pObjMgr = CObjMgr::GetInstance();
	this->m_pUIMgr = CUIMgr::GetInstance();

	m_weaponManager.Initialize();
	CUnit::SetWeaponManager( &m_weaponManager );
	CBuilding::InitBasicBuildTexture();

	this->m_pObjMgr->Initialize();
	CEffectMgr::GetInstance()->Initialize();

	this->m_pUIMgr->Initialize();
	CEntityMgr::GetInstance()->Initialize();
	CResourceMgr::GetInstance()->Initialize();
	CUpgradeMgr::GetInstance()->Initialize();
	CUpgradeMgr::GetInstance()->SetWeaponMgr( &m_weaponManager );

	CResearchMgr::GetInstance()->Initialize();

	this->m_pObjMgr->AddGameObject( CFactory<CBackground>::CreateGameObject(), OBJ_TYPE_BACKGROUND );

	/* Resource Setting.. */
	CResourceMgr::GetInstance()->MakeResource( D3DXVECTOR3( 186.f, 2560.f, 0.f ), CResourceMgr::Resource_Mineral, OBJ_TYPE_RESOURCE );
	CResourceMgr::GetInstance()->MakeResource( D3DXVECTOR3( 186.f, 2592.f, 0.f ), CResourceMgr::Resource_Mineral, OBJ_TYPE_RESOURCE );
	CResourceMgr::GetInstance()->MakeResource( D3DXVECTOR3( 186.f, 2630.f, 0.f ), CResourceMgr::Resource_Mineral, OBJ_TYPE_RESOURCE );
	CResourceMgr::GetInstance()->MakeResource( D3DXVECTOR3( 186.f, 2670.f, 0.f ), CResourceMgr::Resource_Mineral, OBJ_TYPE_RESOURCE );
	CResourceMgr::GetInstance()->MakeResource( D3DXVECTOR3( 186.f, 2702.f, 0.f ), CResourceMgr::Resource_Mineral, OBJ_TYPE_RESOURCE );
	CResourceMgr::GetInstance()->MakeResource( D3DXVECTOR3( 186.f, 2732.f, 0.f ), CResourceMgr::Resource_Mineral, OBJ_TYPE_RESOURCE );
	CResourceMgr::GetInstance()->MakeResource( D3DXVECTOR3( 186.f, 2770.f, 0.f ), CResourceMgr::Resource_Mineral, OBJ_TYPE_RESOURCE );
	CResourceMgr::GetInstance()->MakeResource( D3DXVECTOR3( 186.f, 2806.f, 0.f ), CResourceMgr::Resource_Mineral, OBJ_TYPE_RESOURCE );

	CResourceMgr::GetInstance()->MakeResource( D3DXVECTOR3( 300.f, 2500.f, 0.f ), CResourceMgr::Resource_Gas, OBJ_TYPE_RESOURCE );

	this->m_pObjMgr->AddGameObject( CFactory<CPlayer>::CreateGameObject(), OBJ_TYPE_USER );

	CGameEntity::SetBackground( this->m_pObjMgr->FindGameObject<CBackground>() );

	this->m_pEnemyMgr->Initialize();

	/* Make Command Center.. */
	CGameObject* pObj = CEntityMgr::GetInstance()->MakeUnit(
		CEntityMgr::Entity_Control, D3DXVECTOR3( 368.f, 2672.f, 0.f ), OBJ_TYPE_USER, true );

	this->m_pObjMgr->AddGameObject( pObj, pObj->GetObjectType() );

	CSoundMgr::GetInstance()->StopAll();
	CSoundMgr::GetInstance()->PlayBGM( L"stage.wav", CSoundMgr::Channel_BGM );

	return S_OK;
}

//#include "Mouse.h"
//#include "KeyMgr.h"
int CStage::Update(void)
{
	//if ( CKeyMgr::GetInstance()->GetKeyOnceDown( VK_LBUTTON ) )
	//{
	//	if ( CKeyMgr::GetInstance()->GetKeyStayDown( 'Q' ) )
	//	{
	//		D3DXVECTOR3 vMousePos = CMouse::GetInstance()->GetPos() + CGameObject::GetScroll();
	//		CResourceMgr::GetInstance()->MakeResource( vMousePos, CResourceMgr::Resource_Mineral, OBJ_TYPE_RESOURCE );
	//		cout << "vMousePos.x : " << vMousePos.x << ", vMousePos.y : " << vMousePos.y << endl;
	//	}
	//	else if ( CKeyMgr::GetInstance()->GetKeyStayDown( 'G' ) )
	//	{
	//		D3DXVECTOR3 vMousePos = CMouse::GetInstance()->GetPos() + CGameObject::GetScroll();
	//		CResourceMgr::GetInstance()->MakeResource( vMousePos, CResourceMgr::Resource_Gas, OBJ_TYPE_RESOURCE );
	//		cout << "vMousePos.x : " << vMousePos.x << ", vMousePos.y : " << vMousePos.y << endl;
	//	}
	//}

	this->m_pAStarMgr->Update();
	this->m_pEnemyMgr->Update();
	this->m_pObjMgr->Update();
	this->m_pUIMgr->Update();

	return 0;
}

void CStage::Render(void)
{
	this->m_pObjMgr->Render();

	this->m_pUIMgr->Render();
}

void CStage::Release(void)
{
	CResourceMgr::GetInstance()->DestroyInstance();
	this->m_pObjMgr->Release();
	CEffectMgr::GetInstance()->DestroyInstance();
	this->m_pUIMgr->DestroyInstance();
	CEntityMgr::GetInstance()->DestroyInstance();
	CUpgradeMgr::GetInstance()->DestroyInstance();
	this->m_pAStarMgr->DestroyInstance();
	this->m_pEnemyMgr->DestroyInstance();
	CResearchMgr::GetInstance()->DestroyInstance();
}

CStage::CStage(void)
{
}

CStage::~CStage(void)
{
	Release();
}
