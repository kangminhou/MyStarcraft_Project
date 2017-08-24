#pragma once
#include "GameObject.h"

#include "Corps.h"

class CMouse;
class CMineral;
class CGas;

/* 실제 게임을 조종하는 클래스 ( 실제 게임을 플레이하는 플레이어 ).. */
class CPlayer :
	public CGameObject
{
public:
	typedef struct tagResourceData
	{
		int	iMineral;			// 미네랄 양..
		int iGas;				// 가스 양..
		BYTE byTotalPopulation;	// 총 인구..
		BYTE byCurPopulation;	// 현재 인구..
	}RESOURCE_DATA;

private:
	CCorps	m_clickCorps;			// 현재 클릭한 부대..
	CCorps	m_hotKeyCorps[10];		// 단축키로 지정해놓은 부대..
	CCorps*	m_pCurCorps;

	RESOURCE_DATA	m_tResourceData;	// 플레이어의 리소스 데이터..

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
	// CGameObject을(를) 통해 상속됨
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

