#pragma once
#include "Include.h"

class CAnimation;

/* 마우스 객체 ( 마우스에서 처리할 이벤트들을 담당?? ).. */
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
	MOUSE_DRAG_DATA	m_tDragData;		// 플레이어의 마우스 드래그 데이터..
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
