#pragma once
#include "Component.h"

class CTransform;
class CBackground;
class CGameEntity;
class CAStar;

/* 길찾기를 사용하지 않는 객체들이 움직일 때 사용하는 클래스.. */
class CMove :
	public CComponent
{
public:
	enum eMoveKind
	{
		Move_Destintion,
		Move_ChaseTarget
	};

private:
	CAStar*				m_pAStar;

	CGameEntity*		m_pGameEntity;
	CTransform*			m_pTransform;
	CBackground*		m_pBackground;

	D3DXVECTOR3			m_vDestination;
	D3DXVECTOR3			m_vTilePos;
	CTransform*			m_pTargetTransform;

	eMoveKind			m_eMoveKind;

	std::vector<D3DXVECTOR3>	m_vecMovePath;
	int							m_iCurIndexNum;

public:
	CMove();
	virtual ~CMove();

public:
	// CComponent을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Release() override;

public:
	int Update();
	virtual void SetDestination( const D3DXVECTOR3& _vDestination );

private:
	void SettingMoveData();

	void DecidePathFindMethod();

};

