#include "StdAfx.h"
#include "Stage.h"
#include "TextureMgr.h"
#include "objmgr.h"
#include "Factory.h"

#include "Marine.h"
#include "TestUnit.h"
#include "Background.h"

#include "Include.h"
#include "Random.h"

HRESULT CStage::Initialize(void)
{
	m_weaponManager.Initialize();
	CUnit::SetWeaponManager( &m_weaponManager );

	CObjMgr::GetInstance()->Initialize();

	CObjMgr::GetInstance()->AddGameObject( CFactory<CBackground>::CreateGameObject(), OBJ_TYPE_BACKGROUND );
	
	for ( int i = 0; i < 300; ++i )
	{
		CObjMgr::GetInstance()->AddGameObject( CFactory<CMarine>::CreateGameObject(RANDOM_RANGE_INTERGER(0, 400), RANDOM_RANGE_INTERGER(0, 100)), OBJ_TYPE_USER );
	}

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
