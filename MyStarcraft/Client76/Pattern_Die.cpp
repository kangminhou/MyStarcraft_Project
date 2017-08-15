#include "stdafx.h"
#include "Pattern_Die.h"

#include "GameEntity.h"


CPattern_Die::CPattern_Die()
{
}


CPattern_Die::~CPattern_Die()
{
}

void CPattern_Die::Initialize()
{
	
}

void CPattern_Die::OnEnable()
{
	m_pGameEntity->DieEntity();
}

int CPattern_Die::Update()
{
	return 0;
}

void CPattern_Die::Release()
{
}
