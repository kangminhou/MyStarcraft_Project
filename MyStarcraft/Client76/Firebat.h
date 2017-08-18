#pragma once
#include "Unit.h"

class CFirebat :
	public CUnit
{
public:
	CFirebat();
	virtual ~CFirebat();

public:
	virtual HRESULT Initialize( void ) override;
	virtual int Update( void ) override;
	virtual void Release( void ) override;
	virtual void Render( void ) override;

public:
	// CUnit을(를) 통해 상속됨
	virtual void SetPattern( const eGameEntityPattern & _ePatternKind, const bool & _bPrevPattern = FALSE ) override;

protected:
	virtual void InitAnimation() override;
	virtual void InitPattern() override;

};

