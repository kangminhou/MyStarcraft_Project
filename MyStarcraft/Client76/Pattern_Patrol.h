#pragma once
#include "EntityPattern.h"

class CMove;
class CMouse;

class CPattern_Patrol :
	public CEntityPattern
{
private:
	D3DXVECTOR3 m_vBeginPos;
	D3DXVECTOR3 m_vDestinaion;

	CMove*		m_pMoveComponent;
	CMouse*		m_pMouse;
	
	bool		m_bGoDestination;

public:
	CPattern_Patrol();
	virtual ~CPattern_Patrol();

	// CEntityPattern을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void OnEnable() override;
	virtual int Update() override;
	virtual void Release() override;
};

