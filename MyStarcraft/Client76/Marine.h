#pragma once
#include "Unit.h"

class CMarine :
	public CUnit
{
public:
	CMarine();
	virtual ~CMarine();

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT	Initialize( void ) override;
	virtual int Update( void ) override;
	virtual void Render( void ) override;
	virtual void Release( void ) override;

public:
	// CUnit��(��) ���� ��ӵ�
	virtual void SetPattern( const eGameEntityPattern & _ePatternKind ) override;

protected:
	virtual void InitAnimation() override;
	virtual void InitPattern() override;
};

