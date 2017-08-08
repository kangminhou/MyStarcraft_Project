#pragma once
#include "Component.h"

class CUnitBridge;
class CUnitPattern :
	public CComponent
{
private:
	CUnitBridge*	m_pUnitBridge;

public:
	CUnitPattern();
	virtual ~CUnitPattern();

public:
	void SetUnitBridge( CUnitBridge* _pUnitBridge );

public:
	const CUnitBridge* GetUnitBridge() const;

public:
	// CComponent��(��) ���� ��ӵ�
	virtual void Initialize() PURE;
	virtual void OnEnable() PURE;
	virtual int Update() PURE;
	virtual void Release() PURE;

};

