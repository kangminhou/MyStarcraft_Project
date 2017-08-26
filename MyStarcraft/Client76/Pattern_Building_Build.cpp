#include "stdafx.h"
#include "Pattern_Building_Build.h"

#include "Include.h"
#include "TimeMgr.h"

#include "Building.h"


CPattern_Building_Build::CPattern_Building_Build()
{
}


CPattern_Building_Build::~CPattern_Building_Build()
{
}

void CPattern_Building_Build::Initialize()
{
}

void CPattern_Building_Build::OnEnable()
{
	UNIT_GENERATE_DATA tGenData = this->m_pGameEntity->GetGenerateData();

	this->m_fHpPlusSpeed = this->m_pGameEntity->GetMaxHp() / tGenData.fGenerateTime;
}

int CPattern_Building_Build::Update()
{
	int iOut = CEntityPattern::Event_None;
	float fCurHp = this->m_pGameEntity->GetCurHp() + this->m_fHpPlusSpeed * GET_TIME;

	if ( fCurHp >= this->m_pGameEntity->GetMaxHp() )
	{
		fCurHp = this->m_pGameEntity->GetMaxHp();
		/* ##건물을 다지어서 이벤트 함수를 호출해야 함..*/
		dynamic_cast<CBuilding*>(this->m_pGameEntity)->SuccessBuild();

		this->m_pGameEntity->SetPattern( CGameEntity::Pattern_Idle );
		iOut = CEntityPattern::Event_Pattern_Change;

	}

	this->m_pGameEntity->SetCurHp( fCurHp );

	return iOut;
}

void CPattern_Building_Build::Release()
{
}
