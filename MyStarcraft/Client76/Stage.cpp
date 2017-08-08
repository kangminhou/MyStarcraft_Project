#include "StdAfx.h"
#include "Stage.h"
#include "TextureMgr.h"
#include "objmgr.h"
#include "Factory.h"

#include "Marine.h"

HRESULT CStage::Initialize(void)
{
	m_weaponManager.Initialize();
	CUnit::SetWeaponManager( &m_weaponManager );

	CObjMgr::GetInstance()->Initialize();

	CObjMgr::GetInstance()->AddGameObject( new CMarine, OBJ_TYPE_USER );

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
