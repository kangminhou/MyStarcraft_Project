#pragma once
#include "Unit.h"

class CMarine :
	public CUnit
{
public:
	CMarine();
	virtual ~CMarine();

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT	Initialize( void ) override;
	virtual int Update( void ) override;
	virtual void Render( void ) override;
	virtual void Release( void ) override;

public:
	// CUnit을(를) 통해 상속됨
	virtual void SetPattern( const eGameEntityPattern & _ePatternKind ) override;

protected:
	virtual void InitAnimation() override;
	virtual void InitPattern() override;
};

