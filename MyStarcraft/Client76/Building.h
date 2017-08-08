#pragma once
#include "GameObject.h"
class CBuilding :
	public CGameObject
{
private:
	/* 
	 * 건물은 건설 시의 애니메이션을 가지고 있음..
	 * 
	 */

public:
	CBuilding();
	virtual ~CBuilding();

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT	Initialize(void) override;
	virtual int Update( void ) override;
	virtual void Render( void ) override;
	virtual void Release( void ) override;

};

