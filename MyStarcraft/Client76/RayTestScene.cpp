#include "stdafx.h"
#include "RayTestScene.h"

#include "ObjMgr.h"
#include "Factory.h"

#include "RayTestBackground.h"
#include "RayObject.h"


CRayTestScene::CRayTestScene()
{
}


CRayTestScene::~CRayTestScene()
{
}

HRESULT CRayTestScene::Initialize( void )
{
	CObjMgr::GetInstance()->Initialize();

	CObjMgr::GetInstance()->AddGameObject( CFactory<CRayTestBackground>::CreateGameObject( 0, 0 ), OBJ_TYPE_BACKGROUND );
	CObjMgr::GetInstance()->AddGameObject( CFactory<CRayObject>::CreateGameObject( 0, 0 ), OBJ_TYPE_EFFECT );

	return S_OK;
}

int CRayTestScene::Update( void )
{
	CObjMgr::GetInstance()->Update();

	return 0;
}

void CRayTestScene::Render( void )
{
	CObjMgr::GetInstance()->Render();
}

void CRayTestScene::Release( void )
{
	CObjMgr::GetInstance()->Release();
}
