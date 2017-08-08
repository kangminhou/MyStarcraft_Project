#include "stdafx.h"
#include "UnitPattern.h"

#include "GameObject.h"

CUnitPattern::CUnitPattern()
{
}


CUnitPattern::~CUnitPattern()
{
}

void CUnitPattern::SetUnitBridge( CUnitBridge * _pUnitBridge )
{
	this->m_pUnitBridge = _pUnitBridge;
}

const CUnitBridge * CUnitPattern::GetUnitBridge() const
{
	return this->m_pUnitBridge;
}
