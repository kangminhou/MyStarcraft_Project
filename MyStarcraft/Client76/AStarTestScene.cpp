#include "stdafx.h"
#include "AStarTestScene.h"

#include "ObjMgr.h"
#include "Factory.h"

#include "TestAStar.h"


CAStarTestScene::CAStarTestScene()
{
}


CAStarTestScene::~CAStarTestScene()
{
}

HRESULT CAStarTestScene::Initialize( void )
{
	CObjMgr::GetInstance()->Initialize();

	CObjMgr::GetInstance()->AddGameObject( CFactory<CTestAStar>::CreateGameObject(), OBJ_TYPE_BACKGROUND );

	return S_OK;
}

int CAStarTestScene::Update( void )
{
	CObjMgr::GetInstance()->Update();

	return 0;
}

void CAStarTestScene::Render( void )
{
	CObjMgr::GetInstance()->Render();
}

void CAStarTestScene::Release( void )
{
	CObjMgr::GetInstance()->Release();
}
