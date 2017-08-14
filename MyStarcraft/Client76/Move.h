#pragma once
#include "Component.h"

class CTransform;
class CBackground;
class CGameEntity;
class CAStar;

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
	// CComponent��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void Release() override;

public:
	int Update();
	virtual void SetDestination( const D3DXVECTOR3& _vDestination );

private:
	void SettingMoveData();

	void DecidePathFindMethod();

};

