#pragma once
#include "GameObject.h"

#include "Corps.h"

class CMouse;
/* ���� ������ �����ϴ� Ŭ���� ( ���� ������ �÷����ϴ� �÷��̾� ).. */
class CPlayer :
	public CGameObject
{
public:
	typedef struct tagResourceData
	{
		int	iMineral;			// �̳׶� ��..
		int iGas;				// ���� ��..
		BYTE byTotalPopulation;	// �� �α�..
		BYTE byCurPopulation;	// ���� �α�..
	}RESOURCE_DATA;

private:
	CCorps	m_clickCorps;			// ���� Ŭ���� �δ�..
	CCorps	m_hotKeyCorps[10];		// ����Ű�� �����س��� �δ�..
	CCorps*	m_pCurCorps;

	RESOURCE_DATA	m_tResourceData;	// �÷��̾��� ���ҽ� ������..

	CMouse*			m_pMouse;

public:
	CPlayer();
	virtual ~CPlayer();

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Initialize( void )override;
	virtual int Update( void ) override;
	virtual void Render( void ) override;
	virtual void Release( void ) override;

private:
	void KeyCheck( void );
	void MakeDragUnitCorps();

	void UnitMove();

};

