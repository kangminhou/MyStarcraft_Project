#include "stdafx.h"
#include "EntityPattern.h"

#include "GameObject.h"

CEntityPattern::CEntityPattern()
{
}


CEntityPattern::~CEntityPattern()
{
}

void CEntityPattern::SetGameEntity( CGameEntity * _pGameEntity )
{
	this->m_pGameEntity = _pGameEntity;
}
