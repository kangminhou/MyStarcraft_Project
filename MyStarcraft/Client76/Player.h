#pragma once
#include "GameObject.h"

#include "Corps.h"

class CMouse;
class CMineral;
class CGas;

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

	vector<BUTTON_DATA*>	m_vecCurCanActButton;
	BUTTON_DATA*			m_pCancelButton;

	const BUTTON_DATA*		m_pPushButtonData;
	bool					m_bOrderAct;

	vector<CGameEntity*>	m_vecClickEventEntity;
	vector<CMineral*>		m_vecMineral;
	vector<CGas*>			m_vecGas;

public:
	CPlayer();
	virtual ~CPlayer();

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Initialize( void )override;
	virtual int Update( void ) override;
	virtual void Render( void ) override;
	virtual void Release( void ) override;

public:
	void OrderActPattern( const BUTTON_DATA* _pButtonData );
	void DecideShowButton();
	void ShowOnlyCancelButtonInterface();
	void ShowEntityUI();

	void ResetMouseClickEventEntity();
	void AddMouseClickEventEntity(CGameEntity* _pEntity);
	void EraseMouseClickEventEntity( CGameEntity* _pEntity );

private:
	void KeyCheck( void );
	void MakeDragUnitCorps();

	void RButtonClick();
	void UnitMove();

};

