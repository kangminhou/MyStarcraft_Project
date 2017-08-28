#include "stdafx.h"
#include "Pattern_MakeUnit.h"

#include "TimeMgr.h"
#include "ObjMgr.h"

#include "Building.h"
#include "Player.h"
#include "UIMgr.h"


CPattern_MakeUnit::CPattern_MakeUnit()
	: m_bStop(false)
{
}


CPattern_MakeUnit::~CPattern_MakeUnit()
{
}

void CPattern_MakeUnit::Initialize()
{
	this->m_pBuilding = dynamic_cast<CBuilding*>(this->m_pGameEntity);
	this->m_pPlayer = CObjMgr::GetInstance()->FindGameObject<CPlayer>();
	this->m_pUIMgr = CUIMgr::GetInstance();

	if ( !this->m_pBuilding )
	{
		ERROR_MSG( L"CPattern_Research Initialize Failed ( GameEntity is not Building Class )" );
		return;
	}
}

void CPattern_MakeUnit::OnEnable()
{
	this->m_bStop = false;
	
	if ( !this->Setting() )
	{
		this->m_pGameEntity->SetPattern( CGameEntity::Pattern_Idle );
		return;
	}
}

int CPattern_MakeUnit::Update()
{
	int iReturn = CEntityPattern::Event_None;

	if ( this->m_bStop )
	{
		if ( !this->m_pPlayer->GetFullPopulation() )
		{
			this->m_bStop = false;

			if ( !this->Setting() )
			{
				this->m_pGameEntity->SetPattern( CGameEntity::Pattern_Idle );
				iReturn = CEntityPattern::Event_Pattern_Change;
			}
		}
	}
	else
	{
		this->m_fRestTime -= GET_TIME;

		if ( this->m_fRestTime <= 0.f )
		{
			this->m_fRestTime = 0.f;
			this->m_pBuilding->SuccessOrder( CGameEntity::Pattern_Make_Unit );

			if ( !this->Setting() )
			{
				this->m_pGameEntity->SetPattern( CGameEntity::Pattern_Idle );
				iReturn = CEntityPattern::Event_Pattern_Change;
			}

		}
	}

	BYTE byRatio = (BYTE)(float(this->m_fRestTime / this->m_fMaxTime) * 100.f);
	this->m_pBuilding->SetProgressRatio( byRatio );

	return iReturn;
}

void CPattern_MakeUnit::Release()
{
}

bool CPattern_MakeUnit::Setting()
{
	if ( this->m_pBuilding->GetOrderVectorSize() <= 0 )
		return false;
	else if ( this->m_pPlayer->GetFullPopulation() )
	{
		this->m_bStop = true;
		this->m_pBuilding->SoundPlay( CGameEntity::Sound_ETC, 2 );
		this->m_pUIMgr->ShowFont( CUIMgr::Font_No_Population );
	}

	m_tCurOrderData = this->m_pBuilding->GetOrderData( 0 );
	UNIT_GENERATE_DATA* pGenData = (UNIT_GENERATE_DATA*)this->m_tCurOrderData.pData;
	
	this->m_fMaxTime = this->m_fRestTime = pGenData->fGenerateTime;

	return true;
}
