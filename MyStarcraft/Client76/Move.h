#pragma once
#include "Component.h"

class CTransform;
class CBackground;
class CGameEntity;
class CAStar;
class CAStarManager;

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
	CAStarManager*		m_pAStarManager;
	CAStar*				m_pAStar;

	CGameEntity*		m_pGameEntity;
	CTransform*			m_pTransform;
	CBackground*		m_pBackground;

	D3DXVECTOR3			m_vDestination;
	D3DXVECTOR3			m_vTilePos;
	CTransform*			m_pTargetTransform;

	eMoveKind			m_eMoveKind;

	std::vector<D3DXVECTOR3>	m_vecMovePath;

	int					m_iCurIndexNum;

	float				m_fRestReFindPath;
	float				m_fReFindPathCycle;

	bool				m_bCanRestReFindPath;
	bool				m_bCanMove;
	bool				m_bWaitAStarResult;
	bool				m_bMoveStop;

public:
	CMove();
	virtual ~CMove();

public:
	// CComponent을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Release() override;

public:
	int Update();
	void SetDestination( const D3DXVECTOR3& _vDestination, const bool& _bChaseTarget = false );
	void PathFind();
	void ReFindPath();

	void GetAStarResult(const D3DXVECTOR3& _vDestination, const int& _iReturnEvent );

private:
	void SettingMoveData();

	void DecidePathFindMethod();

};

