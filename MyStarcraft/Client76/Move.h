#pragma once
#include "Component.h"

class CTransform;
class CBackground;
class CGameEntity;
class CAStar;
class CAStarManager;

/* ��ã�⸦ ������� �ʴ� ��ü���� ������ �� ����ϴ� Ŭ����.. */
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
	// CComponent��(��) ���� ��ӵ�
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

