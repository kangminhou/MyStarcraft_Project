#include "stdafx.h"
#include "Pattern_Patrol.h"

#include "GameEntity.h"
#include "Mouse.h"
#include "Move.h"


CPattern_Patrol::CPattern_Patrol()
{
}


CPattern_Patrol::~CPattern_Patrol()
{
}

void CPattern_Patrol::Initialize()
{
	this->m_pMoveComponent = const_cast<CMove*>(this->m_pGameEntity->GetComponent<CMove>());
	this->m_pMouse = CMouse::GetInstance();
}

void CPattern_Patrol::OnEnable()
{
	this->m_vBeginPos = this->m_pGameEntity->GetPos();

	D3DXVECTOR3 vMouse = this->m_pMouse->GetPos();
	vMouse += this->m_pGameEntity->GetScroll();

	this->m_vDestinaion = vMouse;

	this->m_pMoveComponent->SetDestination( this->m_vDestinaion );
	this->m_bGoDestination = true;
}

int CPattern_Patrol::Update()
{
	int iEvent = this->m_pMoveComponent->Update();

	if ( iEvent == 1 )
	{
		if ( this->m_bGoDestination )
			this->m_pMoveComponent->SetDestination( this->m_vBeginPos );
		else
			this->m_pMoveComponent->SetDestination( this->m_vDestinaion );

		this->m_bGoDestination = ((this->m_bGoDestination) ? false : true);
	}

	return 0;
}

void CPattern_Patrol::Release()
{
}
