#include "stdafx.h"
#include "Corps.h"


CCorps::CCorps()
{
}


CCorps::~CCorps()
{
}

BYTE CCorps::GetCurUnitNum() const
{
	return this->m_byCurUnitNum;
}

void CCorps::Initialize()
{
	m_byCurUnitNum = 0;

	for (int i = 0; i < MAX_UNIT; ++i)
		m_pEntityArr[i] = NULL;
}

void CCorps::Release()
{
}

void CCorps::Render()
{
	for ( int i = 0; i < this->m_byCurUnitNum; ++i )
	{
		this->m_pEntityArr[i]->RenderSelectTexture( true );
	}
}

void CCorps::SetUnitPattern( CUnit::eGameEntityPattern _ePatternKind )
{
	for ( int i = 0; i < this->m_byCurUnitNum; ++i )
	{
		if ( !this->m_pEntityArr[i] )
			continue;

		this->m_pEntityArr[i]->SetPattern( _ePatternKind );
	}
}

void CCorps::AddUnit( CGameEntity * _pEntity )
{
	if ( _pEntity )
	{
		this->m_pEntityArr[this->m_byCurUnitNum] = _pEntity;
		++this->m_byCurUnitNum;
	}
}

void CCorps::ResetCorps( void )
{
	for ( int i = 0; i < this->m_byCurUnitNum; ++i )
	{
		if ( this->m_pEntityArr )
			this->m_pEntityArr[i] = NULL;
	}

	this->m_byCurUnitNum = 0;
}
