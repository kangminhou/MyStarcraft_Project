#pragma once
#include "GameObject.h"

#include "Corps.h"

class CMouse;
class CMineral;
class CGas;
class CUIMgr;
class CUpgradeMgr;
class CObjMgr;
class CKeyMgr;

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
	CObjMgr*	m_pObjMgr;
	CKeyMgr*	m_pKeyMgr;

	CCorps	m_clickCorps;			// ���� Ŭ���� �δ�..
	CCorps	m_hotKeyCorps[10];		// ����Ű�� �����س��� �δ�..
	CCorps*	m_pCurCorps;

	CUIMgr*			m_pUIMgr;
	CUpgradeMgr*	m_pUpgradeMgr;

	RESOURCE_DATA	m_tResourceData;	// �÷��̾��� ���ҽ� ������..

	CMouse*			m_pMouse;

	vector<BUTTON_DATA*>	m_vecCurCanActButton;
	vector<BUTTON_DATA*>	m_vecCancelButton;

	const BUTTON_DATA*		m_pPushButtonData;
	bool					m_bOrderAct;
	bool					m_bDoubleClick;
	bool					m_bWaitAct;

	vector<CGameEntity*>	m_vecClickEventEntity;
	vector<CMineral*>		m_vecMineral;
	vector<CGas*>			m_vecGas;

	BYTE					m_byCnt;

public:
	CPlayer();
	virtual ~CPlayer();

public:
	void SetTotalPopulation( const BYTE& _byTotalPopulation );
	void GatherMoney( const int& _iMoney );
	void GahterGas( const int& _iGas );

public:
	RESOURCE_DATA GetResourceData() const;
	BYTE GetTotalPopulation() const;
	bool GetFullPopulation() const;

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Initialize( void )override;
	virtual int Update( void ) override;
	virtual void Render( void ) override;
	virtual void Release( void ) override;

public:
	bool CheckNearMineral( CMineral*& _pOutMineral, CGameEntity* _pEntity );

public:
	void OrderActPattern( const BUTTON_DATA* _pButtonData );
	void DecideShowButton();
	void ShowOnlyCancelButtonInterface();
	void ShowEntityUI();

	void ResetMouseClickEventEntity();
	void AddMouseClickEventEntity(CGameEntity* _pEntity);
	void EraseMouseClickEventEntity( CGameEntity* _pEntity );

	bool BuyUnit( const UNIT_GENERATE_DATA& _tUnitGenData );
	bool BuyUnit( const RESEARCH_DATA& _tResearchData );
	bool CheckCanMakeUnit( const UNIT_GENERATE_DATA& _tUnitGenData );
	int CheckCanBuyUnit( const UNIT_GENERATE_DATA& _tUnitGenData );
	int CheckCanBuyUnit( const RESEARCH_DATA& _tResearchData );

	void SetScroll( const D3DXVECTOR3& _vScroll );

private:
	void KeyCheck( void );
	void MakeDragUnitCorps();

	void RButtonClick();
	void UnitMove();

	void ActCommand();

};

