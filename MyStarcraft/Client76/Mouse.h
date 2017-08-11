#pragma once
#include "Include.h"

class CAnimation;

/* ���콺 ��ü ( ���콺���� ó���� �̺�Ʈ���� ���?? ).. */
class CMouse
{
	DECLARE_SINGLETON( CMouse )

private:
	enum eMouseState
	{
		State_Idle,
		State_Drag,

	};

private:
	MOUSE_DRAG_DATA	m_tDragData;		// �÷��̾��� ���콺 �巡�� ������..
	D3DXVECTOR3		m_vPos;

	vector<const TEX_INFO*>	m_vecTexture;
	const TEX_INFO*			m_pDragTexture;
	CAnimation*				m_pAnimCom;

	eMouseState				m_eState;

	bool					m_bMouseClick;

private:
	CMouse();
	~CMouse();

public:
	MOUSE_DRAG_DATA GetDragData() const;
	D3DXVECTOR3 GetPos() const;

public:
	virtual HRESULT Initialize( void );
	virtual int Update( void );
	virtual void Render( void );
	virtual void Release( void );

private:
	void CheckDragData();
	void UpdatePosition();
	void UpdateMouseEvent();

	void DragAreaRender();

};
