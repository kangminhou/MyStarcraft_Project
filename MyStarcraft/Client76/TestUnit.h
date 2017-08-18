#pragma once
#include "Unit.h"
class CTestUnit :
	public CUnit
{
public:
	CTestUnit();
	virtual ~CTestUnit();

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT	Initialize( void ) override;
	virtual int Update( void ) override;
	virtual void Release( void ) override;

public:
	// CUnit��(��) ���� ��ӵ�
	virtual void SetPattern( const eGameEntityPattern & _ePatternKind, const bool & _bPrevPattern = FALSE ) override;

protected:
	virtual void InitAnimation() override;
	virtual void InitPattern() override;

};

