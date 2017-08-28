#include "stdafx.h"
#include "FogOfWar_Test.h"

#include "ObjMgr.h"

#include "FogBackground.h"
#include "FogUnit.h"
#include "Factory.h"

#include "Random.h"


CFogOfWar_Test::CFogOfWar_Test()
{
}


CFogOfWar_Test::~CFogOfWar_Test()
{
	Release();
}

HRESULT CFogOfWar_Test::Initialize( void )
{
	CRandom::GetInstance()->Initialize( 0 );

	for ( int i = 0; i < 3; ++i )
	{
		float fRandomX = CRandom::GetInstance()->GetFloatValue( 0.f, 800.f );
		float fRandomY = CRandom::GetInstance()->GetFloatValue( 0.f, 600.f );

		//fRandomX = 500.f;
		//fRandomY = 500.f;

		CObjMgr::GetInstance()->AddGameObject( CFactory<CFogUnit>::CreateGameObject( fRandomX, fRandomY ), OBJ_TYPE_RESOURCE );
	}
	CObjMgr::GetInstance()->AddGameObject( CFactory<CFogBackground>::CreateGameObject(), OBJ_TYPE_BACKGROUND);

	return S_OK;
}

int CFogOfWar_Test::Update( void )
{
	CObjMgr::GetInstance()->Update();

	return 0;
}

void CFogOfWar_Test::Render( void )
{
	CObjMgr::GetInstance()->Render();

}

void CFogOfWar_Test::Release( void )
{
	CObjMgr::GetInstance()->Release();
}
