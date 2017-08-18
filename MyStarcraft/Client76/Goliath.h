#pragma once
#include "Unit.h"

class CGoliath :
	public CUnit
{
public:
	CGoliath();
	virtual ~CGoliath();

public:
	virtual HRESULT Initialize( void ) override;
	virtual int Update( void ) override;
	virtual void Render( void ) override;
	virtual void Release( void ) override;

protected:
	virtual void InitAnimation() override;
	virtual void InitPattern() override;

public:
	virtual void SetPattern( const eGameEntityPattern& _ePatternKind, const bool& _bPrevPattern = FALSE ) override;

};

