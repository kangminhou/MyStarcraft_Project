#include "stdafx.h"
#include "Corps.h"


CCorps::CCorps()
{
}


CCorps::~CCorps()
{
}

RECT CCorps::GetEntityIncludeRect() const
{
	return this->m_rcEntityInclude;
}

BYTE CCorps::GetCurUnitNum() const
{
	return this->m_byCurUnitNum;
}

D3DXVECTOR3 CCorps::GetCenterPos() const
{
	return this->m_vCenterPos;
}

bool CCorps::GetGatherEntitys() const
{
	return this->m_bGatherEntitys;
}

void CCorps::Initialize()
{
	this->m_eCurPattern = CGameEntity::Pattern_Idle;
	this->m_byCurUnitNum = 0;

	for (int i = 0; i < MAX_UNIT; ++i)
		this->m_pEntityArr[i] = NULL;
}

void CCorps::Release()
{
}

void CCorps::Update()
{
}

void CCorps::Render()
{
	for ( int i = 0; i < this->m_byCurUnitNum; ++i )
	{
		this->m_pEntityArr[i]->RenderSelectTexture( true );
	}
}

void CCorps::SetUnitPattern( const CUnit::eGameEntityPattern & _ePatternKind )
{
	for ( int i = 0; i < this->m_byCurUnitNum; ++i )
	{
		if ( !this->m_pEntityArr[i] )
			continue;

		this->m_eCurPattern = _ePatternKind;

		if ( this->m_eCurPattern == CGameEntity::Pattern_Move )
		{	
			this->CalcCorpsMoveKind();
		}

		this->m_pEntityArr[i]->SetPattern( _ePatternKind );

	}
}

void CCorps::AddUnit( CGameEntity * _pEntity )
{
	if ( _pEntity )
	{
		this->m_pEntityArr[this->m_byCurUnitNum] = _pEntity;
		this->m_pEntityArr[this->m_byCurUnitNum]->SetEntityBelongToCorps( this );
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

void CCorps::CalcCorpsMoveKind()
{
	D3DXVECTOR3 vTotalPos( 0.f, 0.f, 0.f );
	float fMinX = INFINITY, fMinY = INFINITY;
	float fMaxX = -INFINITY, fMaxY = -INFINITY;
	for ( int i = 0; i < this->m_byCurUnitNum; ++i )
	{
		if ( this->m_pEntityArr[i] )
		{
			D3DXVECTOR3 vEntityPos = this->m_pEntityArr[i]->GetPos();
			vTotalPos += vEntityPos;

			if ( vEntityPos.x < fMinX )
				fMinX = vEntityPos.x;
			if ( vEntityPos.x > fMaxX)
				fMaxX = vEntityPos.x;
			if ( vEntityPos.y < fMinY )
				fMinY = vEntityPos.y;
			if ( vEntityPos.y > fMaxY )
				fMaxY = vEntityPos.y;
		}
	}

	this->m_vCenterPos = vTotalPos / this->m_byCurUnitNum;
	this->m_bGatherEntitys = false;

	RECT rc = { fMinX, fMinY, fMaxX, fMaxY };
	this->m_rcEntityInclude = rc;

	for ( int i = 0; i < this->m_byCurUnitNum; ++i )
	{
		if ( this->m_pEntityArr[i] )
		{
			if ( D3DXVec3Length( &(this->m_vCenterPos - this->m_pEntityArr[i]->GetPos()) ) >= 100.f )
			{
				this->m_bGatherEntitys = true;
				break;
			}
		}
	}
}
