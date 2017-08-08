#include "stdafx.h"
#include "Component.h"


CComponent::CComponent()
{
}


CComponent::~CComponent()
{
}

void CComponent::SetActive( const bool & _bActive )
{
	m_bActive = _bActive;
}

void CComponent::SetGameObject( CGameObject * pGameObject )
{
	this->m_pGameObject = pGameObject;
}

bool CComponent::GetActive() const
{
	return m_bActive;
}

const CGameObject * CComponent::GetGameObject() const
{
	return m_pGameObject;
}
