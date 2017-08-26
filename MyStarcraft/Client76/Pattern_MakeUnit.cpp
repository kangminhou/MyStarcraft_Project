#include "stdafx.h"
#include "Pattern_MakeUnit.h"

#include "TimeMgr.h"

#include "Building.h"


CPattern_MakeUnit::CPattern_MakeUnit()
{
}


CPattern_MakeUnit::~CPattern_MakeUnit()
{
}

void CPattern_MakeUnit::Initialize()
{
	this->m_pBuilding = dynamic_cast<CBuilding*>(this->m_pGameEntity);

	if ( !this->m_pBuilding )
	{
		ERROR_MSG( L"CPattern_Research Initialize Failed ( GameEntity is not Building Class )" );
		return;
	}
}

void CPattern_MakeUnit::OnEnable()
{
	if ( !this->Setting() )
	{
		this->m_pGameEntity->SetPattern( CGameEntity::Pattern_Idle );
		return;
	}
}

int CPattern_MakeUnit::Update()
{
	this->m_fRestTime -= GET_TIME;

	if ( this->m_fRestTime <= 0.f )
	{
		this->m_pBuilding->SuccessOrder( CGameEntity::Pattern_Make_Unit );

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

void CPattern_MakeUnit::Release()
{
}

bool CPattern_MakeUnit::Setting()
{
	if ( this->m_pBuilding->GetOrderVectorSize() <= 0 )
		return false;

	m_tCurOrderData = this->m_pBuilding->GetOrderData( 0 );
	UNIT_GENERATE_DATA* pGenData = (UNIT_GENERATE_DATA*)this->m_tCurOrderData.pData;
	
	this->m_fMaxTime = this->m_fRestTime = pGenData->fGenerateTime;

}
