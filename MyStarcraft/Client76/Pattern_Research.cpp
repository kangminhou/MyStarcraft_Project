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

	if ( !this->Setting() )
	{
		this->m_pGameEntity->SetPattern( CGameEntity::Pattern_Idle );
		return;
	}

	//if ( eKind < 0 || eKind >= Research_End )
	//{
	//	ERROR_MSG( L"CPattern_Research OnEnable Failed ( eKind Out Range )" );
	//	return;
	//}
	//
	//RESEARCH_DATA tResearchData = this->m_pResearchMgr->GetResearchData( eKind );
	//
	//if ( !this->m_pPlayer->BuyUnit( 
	//	tagUnitGenerateData( tResearchData.fResearchTime, tResearchData.iRequireMineral, tResearchData.iRequireGas, 0, 0 ) ) )
	//{
	//	this->m_pGameEntity->SetPattern( CGameEntity::Pattern_Idle );
	//	return;
	//}
	//
	//this->m_fRestTime = tResearchData.fResearchTime;
}

int CPattern_Research::Update()
{
	this->m_fRestTime -= GET_TIME;

	if ( this->m_fRestTime <= 0.f )
	{
		//this->m_pBuilding->SuccessOrder( CGameEntity::Pattern_Make_Unit );
		this->m_pBuilding->SuccessOrder( CGameEntity::Pattern_Research );

		if ( !this->Setting() )
		{
			this->m_pGameEntity->SetPattern( CGameEntity::Pattern_Idle );
			return CEntityPattern::Event_Pattern_Change;
		}

	}
	else
	{
		BYTE byRatio = (BYTE)((this->m_fRestTime / this->m_fMaxTime) * 100.f);
		this->m_pBuilding->SetProgressRatio( byRatio );
	}

	return CEntityPattern::Event_None;
}

void CPattern_Research::Release()
{
}

bool CPattern_Research::Setting()
{
	if ( this->m_pBuilding->GetOrderVectorSize() <= 0 )
		return false;

	m_tCurOrderData = this->m_pBuilding->GetOrderData( 0 );
	RESEARCH_DATA* pGenData = (RESEARCH_DATA                                                                                                                                                                   *   )this->m_tCurOrderData.pData;

	this->m_fMaxTime = this->m_fRestTime = pGenData->fResearchTime;

	return true;
}
