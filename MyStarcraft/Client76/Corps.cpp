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

void CCorps::SetUnitPattern( CUnit::eGameEntityPattern _ePatternKind )
{
	for ( int i = 0; i < this->m_byCurUnitNum; ++i )
	{
		if ( !this->m_pEntityArr[i] )
			continue;

		this->m_eCurPattern = _ePatternKind;

		if ( this->m_eCurPattern == CGameEntity::Pattern_Move )
		{
			D3DXVECTOR3 vTotalPos( 0.f, 0.f, 0.f );
			for ( int i = 0; i < this->m_byCurUnitNum; ++i )
			{
				if ( this->m_pEntityArr[i] )
					vTotalPos += this->m_pEntityArr[i]->GetPos();
			}

			this->m_vCenterPos = vTotalPos / this->m_byCurUnitNum;
			this->m_bGatherEntitys = false;

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
