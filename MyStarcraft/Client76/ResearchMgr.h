#pragma once
#include "Include.h"

class CEntityMgr;

class CResearchMgr
{
	DECLARE_SINGLETON( CResearchMgr )

private:
	pair<RESEARCH_DATA, bool> m_ResearchDataArr[Research_End];

	CEntityMgr*	m_pEntityMgr;

public:
	CResearchMgr();
	~CResearchMgr();

public:
	bool GetIsSuccessResearch( const eResearchKind& _eResearchKind );
	RESEARCH_DATA GetResearchData( const eResearchKind& _eResearchKind );
	
public:
	void Initialize();
	void Release();

public:
	void SuccessResearch( const eResearchKind& _eResearchKind );

};

