#pragma once
#include "Include.h"

class CAnimation;
class CObjMgr;
class CPlayer;

/* ���콺 ��ü ( ���콺���� ó���� �̺�Ʈ���� ���?? ).. */
class CMouse
{
	DECLARE_SINGLETON( CMouse )

private:
	enum eMouseState
	{
		State_Idle,
		State_Drag,
		State_HoverUnit,
		State_Scroll,
		State_SelectTarget,
		State_End,
	};

private:
	CObjMgr*		m_pObjMgr;
	LPD3DXSPRITE	m_pSprite;
	CPlayer*		m_pPlayer;

	MOUSE_DRAG_DATA	m_tDragData;		// �÷��̾��� ���콺 �巡�� ������..
	MOUSE_DRAG_DATA	m_tCheckStateDragData;		// �÷��̾��� ���콺 �巡�� ������..
	D3DXVECTOR3		m_vPos;

	map<wstring, vector<const TEX_INFO*>>	m_mapAllTextureArr[State_End];
	vector<const TEX_INFO*>	m_vecCurTexture;

	wstring					m_wstrDetailStateKey;

	const TEX_INFO*			m_pDragTexture;

	CAnimation*				m_pAnimCom;

	eMouseState				m_eState;

	D3DXMATRIX				m_matWorld;

	bool					m_bMouseClick;
	bool					m_bMinimapClick;
#ifdef _IMAGE_THREAD_LOADING
	bool					m_bWaitTextureLoading;
#endif

	float					m_fWaitCheckStateTime;
	D3DXVECTOR3				m_vScrollMoveSpeed;

private:
	CMouse();
	~CMouse();

public:
	void SetMinimapClick( const bool& _bMinimapClick );
	void SetPlayer( CPlayer* _pPlayer );

public:
	bool GetMinimapClick() const;
	MOUSE_DRAG_DATA GetDragData() const;
	D3DXVECTOR3 GetPos() const;

public:
	virtual HRESULT Initialize( void );
	virtual int Update( void );
	virtual void Render( void );
	virtual void Release( void );

public:
	void ActSelectTargetState();
	void ActIdleState();

private:
	void CheckDragData();
	void UpdatePosition();
	void UpdateMouseEvent();

	void ScrollMoveCheck();

	void DragAreaRender();

	void DecideMouseState();
	void DecideMouseTexture();

	void InitTexture();
	void InitAnimation();

};
