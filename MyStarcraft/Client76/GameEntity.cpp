#include "stdafx.h"
#include "GameEntity.h"

#include "Animation.h"


CGameEntity::CGameEntity()
{
}


CGameEntity::~CGameEntity()
{
}

HRESULT CGameEntity::Initialize( void )
{
	this->m_pAnimCom = new CAnimation;
	this->m_pAnimCom->Initialize();

	this->InitAnimation();
	this->InitPattern();

	return S_OK;
}

int CGameEntity::Update( void )
{
	return 0;
}

void CGameEntity::Render( void )
{
}

void CGameEntity::Release( void )
{
}
