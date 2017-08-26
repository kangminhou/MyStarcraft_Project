#include "stdafx.h"
#include "Pattern_Research.h"

#include "ResearchMgr.h"
#include "ObjMgr.h"
#include "TimeMgr.h"

#include "Building.h"
#include "Player.h"


CPattern_Research::CPattern_Research()
{
}


CPattern_Research::~CPattern_Research()
{
}

void CPattern_Research::Initialize()
{
	this->m_pBuilding = dynamic_cast<CBuilding*>(this->m_pGameEntity);
	this->m_pPlayer = CObjMgr::GetInstance()->FindGameObject<CPlayer>();

	this->m_pResearchMgr = CResearchMgr::GetInstance();

	if ( !this->m_pBuilding )
	{
		ERROR_MSG( L"CPattern_Research Initialize Failed ( GameEntity is not Building Class )" );
		return;
	}
}

void CPattern_Research::OnEnable()
{
	eResearchKind eKind = m_pBuilding->GetResearchKind();

	if ( eKind < 0 || eKind >= Research_End )
	{
		ERROR_MSG( L"CPattern_Research OnEnable Failed ( eKind Out Range )" );
		return;
	}

	RESEARCH_DATA tResearchData = this->m_pResearchMgr->GetResearchData( eKind );
	
	if ( !this->m_pPlayer->BuyUnit( 
		tagUnitGenerateData( tResearchData.fResearchTime, tResearchData.iRequireMineral, tResearchData.iRequireGas, 0, 0 ) ) )
	{
		this->m_pGameEntity->SetPattern( CGameEntity::Pattern_Idle );
		return;
	}

	this->m_fRestTime = tResearchData.fResearchTime;
}

int CPattern_Research::Update()
{
	CBuilding::ORDER_DATA tOrderData = this->m_pBuilding->GetOrderData( 0 );

	this->m_fRestTime = ((RESEARCH_DATA*)(tOrderData.pData))->fResearchTime;
	this->m_fRestTime -= GET_TIME;

	if ( this->m_fRestTime <= 0.f )
	{
		this->m_pBuilding->SuccessResearch();
		this->m_pGameEntity->SetPattern( CGameEntity::Pattern_Idle );
		return CEntityPattern::Event_Pattern_Change;
	}
	else
	{

	}

	return CEntityPattern::Event_None;
}

void CPattern_Research::Release()
{
}
