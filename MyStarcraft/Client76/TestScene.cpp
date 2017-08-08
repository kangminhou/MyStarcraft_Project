#include "stdafx.h"
#include "TestScene.h"

#include "ObjMgr.h"
#include "Factory.h"
#include "UnitFactory.h"
#include "TextureMgr.h"
#include "TimeMgr.h"

#include "Unit.h"

CTestScene::CTestScene()
{
}


CTestScene::~CTestScene()
{
}

HRESULT CTestScene::Initialize( void )
{
	CObjMgr::GetInstance()->Initialize();

	//CGameObject* pUnit = CUnitFactory<CUnitBridge>::CreateUnit();

	return S_OK;
}

int CTestScene::Update( void )
{
	CObjMgr::GetInstance()->Update();

	return 0;
}

void CTestScene::Render( void )
{
	CObjMgr::GetInstance()->Render();
}

void CTestScene::Release( void )
{
	
}
