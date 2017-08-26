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

bool CCorps::GetSameUnit() const
{
	return this->m_bSameEntity;
}

CGameEntity * CCorps::GetEntity( const BYTE& _byUnitIndex )
{
	if ( _byUnitIndex >= m_byCurUnitNum )
		return NULL;

	return m_pEntityArr[_byUnitIndex];
}

void CCorps::Initialize()
{
	this->m_eCurPattern = CGameEntity::Pattern_Idle;
	this->m_byCurUnitNum = 0;

	for (int i = 0; i < MAX_UNIT; ++i)
		this->m_pEntityArr[i] = NULL;

	this->m_pPushData = NULL;
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
		if(this->m_pEntityArr[i] )
			this->m_pEntityArr[i]->RenderSelectTexture( true );
	}
}

void CCorps::SetUnitPattern( const CGameEntity::eGameEntityPattern & _ePatternKind )
{
	for ( int i = 0; i < this->m_byCurUnitNum; ++i )
	{
		if ( !this->m_pEntityArr[i] )
			continue;

		this->m_eCurPattern = _ePatternKind;

		if ( this->m_eCurPattern == CGameEntity::Pattern_Move || this->m_eCurPattern == CGameEntity::Pattern_Patrol ||
			 this->m_eCurPattern == CGameEntity::Pattern_MoveAlert)
		{
			this->CalcCorpsMoveKind();
		}

		if ( this->m_pPushData )
			this->m_pEntityArr[i]->PushMessage( this->m_pPushData );

		this->m_pEntityArr[i]->SetPattern( _ePatternKind );

	}

	if ( this->m_pPushData )
		this->m_pPushData = NULL;
	
}

void CCorps::PushMessage( const BUTTON_DATA* pButtonData )
{
	this->m_pPushData = pButtonData;
}

void CCorps::SetUnitSkill( const CGameEntity::eGameEntitySkillKind & _eSkillKind )
{
	for ( int i = 0; i < this->m_byCurUnitNum; ++i )
	{
		if ( !this->m_pEntityArr[i] )
			continue;

		if ( this->m_pPushData )
			this->m_pEntityArr[i]->PushMessage( this->m_pPushData );

		this->m_pEntityArr[i]->UseSkill( _eSkillKind, NULL );

	}

	if ( this->m_pPushData )
		this->m_pPushData = NULL;
}

void CCorps::AddUnit( CGameEntity * _pEntity )
{
	if ( _pEntity )
	{
		this->m_pEntityArr[this->m_byCurUnitNum] = _pEntity;
		this->m_pEntityArr[this->m_byCurUnitNum]->SetEntityBelongToCorps( this );
		++this->m_byCurUnitNum;

		for ( size_t i = 0; i < this->m_byCurUnitNum - 1; ++i )
		{
			if ( typeid(*_pEntity) != typeid(*this->m_pEntityArr[i]) )
				return;
		}

		this->m_bSameEntity = true;

	}
}

void CCorps::EraseUnit( CGameEntity * _pEntity )
{
	bool bFind = false;
	for ( int i = 0; i < this->m_byCurUnitNum; ++i )
	{
		if ( bFind )
		{
			this->m_pEntityArr[i] = this->m_pEntityArr[i + 1];
		}
		else if ( this->m_pEntityArr[i] == _pEntity )
		{
			bFind = true;
			this->m_byCurUnitNum--;
			if ( i == this->m_byCurUnitNum )
				break;

			this->m_pEntityArr[i]->SetEntityBelongToCorps( NULL );
			this->m_pEntityArr[i] = this->m_pEntityArr[i + 1];
		}
	}
}

void CCorps::ResetCorps( void )
{
	for ( int i = 0; i < this->m_byCurUnitNum; ++i )
	{
		if ( this->m_pEntityArr[i] )
		{
			this->m_pEntityArr[i]->SetEntityBelongToCorps( NULL );
			this->m_pEntityArr[i] = NULL;
		}
	}

	this->m_byCurUnitNum = 0;
}

int CCorps::CheckCorpsOnePlaceMove( const D3DXVECTOR3& _vDestination, const CGameEntity* _pEntity ) const
{
	D3DXVECTOR3 vEntityPos = _pEntity->GetPos();
	if ( this->m_rcEntityInclude.left <= _vDestination.x &&
		 this->m_rcEntityInclude.top <= _vDestination.y &&
		 this->m_rcEntityInclude.right >= _vDestination.x &&
		 this->m_rcEntityInclude.bottom >= _vDestination.y)
	{
		if ( _vDestination.x >= vEntityPos.x && _vDestination.y >= vEntityPos.x )
			return 1;
		else if ( _vDestination.x <= vEntityPos.x && _vDestination.y >= vEntityPos.x )
			return 2;
		else if ( _vDestination.x >= vEntityPos.x && _vDestination.y <= vEntityPos.x )
			return 3;
		else
			return 4;
	}

	return 0;
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
