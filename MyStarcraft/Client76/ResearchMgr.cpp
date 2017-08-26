#include "stdafx.h"
#include "ResearchMgr.h"

#include "EntityMgr.h"

IMPLEMENT_SINGLETON( CResearchMgr )


CResearchMgr::CResearchMgr()
{
}


CResearchMgr::~CResearchMgr()
{
}

bool CResearchMgr::GetIsSuccessResearch( const eResearchKind & _eResearchKind )
{
	if ( _eResearchKind < 0 || _eResearchKind >= Research_End )
	{
		ERROR_MSG( L"CResearchMgr GetIsSuccessResearch Failed ( _eResearchKind Out Range )" );
		return false;
	}

	return this->m_ResearchDataArr[_eResearchKind].second;
}

RESEARCH_DATA CResearchMgr::GetResearchData( const eResearchKind & _eResearchKind )
{
	if ( _eResearchKind < 0 || _eResearchKind >= Research_End )
	{
		ERROR_MSG( L"CResearchMgr GetResearchData Failed ( _eResearchKind Out Range )" );
		return RESEARCH_DATA();
	}

	return this->m_ResearchDataArr[_eResearchKind].first;
}

void CResearchMgr::Initialize()
{
	this->m_pEntityMgr = CEntityMgr::GetInstance();

	for ( int i = 0; i < Research_End; ++i )
	{
		this->m_ResearchDataArr[i].second = false;
	}

	this->m_ResearchDataArr[Research_SteamPack].first = RESEARCH_DATA( 10.f, 100, 100, L"Steam pack" );
	//this->m_ResearchDataArr[Research_SiegeTank].first = RESEARCH_DATA( 10.f, 100, 100, L"Steam pack" );
}

void CResearchMgr::Release()
{
}

void CResearchMgr::SuccessResearch( const eResearchKind & _eResearchKind )
{
	if ( _eResearchKind < 0 || _eResearchKind >= Research_End )
	{
		ERROR_MSG( L"CResearchMgr SuccessResearch Failed ( _eResearchKind Out Range )" );
		return;
	}

	if ( !this->m_ResearchDataArr[_eResearchKind].second )
	{
		this->m_ResearchDataArr[_eResearchKind].second = true;

		this->m_pEntityMgr->SuccessResearch( this->m_ResearchDataArr[_eResearchKind].first.wstrName );
	}
}
