#include "StdAfx.h"
#include "Mouse.h"

#include "KeyMgr.h"
#include "TextureMgr.h"
#include "Device.h"

#include "GameObject.h"

IMPLEMENT_SINGLETON(CMouse)

CMouse::CMouse()
	: m_bMouseClick(false)
{
}

CMouse::~CMouse()
{
	Release();
}

MOUSE_DRAG_DATA CMouse::GetDragData() const
{
	return this->m_tDragData;
}

D3DXVECTOR3 CMouse::GetPos() const
{
	return m_vPos;
}

HRESULT CMouse::Initialize( void )
{
	m_pDragTexture = CTextureMgr::GetInstance()->GetTexture( L"Drag" );

	m_eState = State_Idle;

	return S_OK;
}

int CMouse::Update( void )
{
	this->UpdatePosition();
	this->UpdateMouseEvent();

	return 0;
}

void CMouse::Render( void )
{
	switch ( m_eState )
	{
		case State_Idle:
			break;
		case State_Drag:
			this->DragAreaRender();
			break;
	}
	if ( m_bMouseClick )
	{

	}
}

void CMouse::Release( void )
{
}

void CMouse::CheckDragData()
{
	if ( this->m_tDragData.vStartPos.x > this->m_tDragData.vEndPos.x )
	{
		float fTemp = this->m_tDragData.vStartPos.x;
		this->m_tDragData.vStartPos.x = this->m_tDragData.vEndPos.x;
		this->m_tDragData.vEndPos.x = fTemp;
	}

	if ( this->m_tDragData.vStartPos.y > this->m_tDragData.vEndPos.y )
	{
		float fTemp = this->m_tDragData.vStartPos.y;
		this->m_tDragData.vStartPos.y = this->m_tDragData.vEndPos.y;
		this->m_tDragData.vEndPos.y = fTemp;
	}
}

void CMouse::UpdatePosition()
{
	POINT ptMouse;

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	m_vPos = D3DXVECTOR3( (FLOAT)ptMouse.x, (FLOAT)ptMouse.y, 0.f );
}

void CMouse::UpdateMouseEvent()
{
	if ( CKeyMgr::GetInstance()->GetKeyOnceDown( VK_LBUTTON ) )
	{
		this->m_bMouseClick = true;
		this->m_tDragData.vStartPos = this->m_vPos;
	}
	if ( CKeyMgr::GetInstance()->GetKeyStayDown( VK_LBUTTON ) )
	{
		this->m_tDragData.vEndPos = this->m_vPos;
		if ( this->m_tDragData.vEndPos != this->m_tDragData.vStartPos && m_eState != State_Drag )
			this->m_eState = State_Drag;
	}
	if ( CKeyMgr::GetInstance()->GetKeyUp( VK_LBUTTON ) )
	{
		this->m_bMouseClick = false;
		this->CheckDragData();

		this->m_eState = State_Idle;
	}
}

void CMouse::DragAreaRender()
{
	D3DXMATRIX matTrans, matScale, matWorld;

	D3DXVECTOR3 vPoint[4];

	vPoint[0] = D3DXVECTOR3( m_tDragData.vStartPos.x, m_tDragData.vStartPos.y, 0.f );
	vPoint[1] = D3DXVECTOR3( m_tDragData.vStartPos.x, m_tDragData.vStartPos.y, 0.f );
	vPoint[2] = D3DXVECTOR3( m_tDragData.vStartPos.x, m_tDragData.vEndPos.y, 0.f );
	vPoint[3] = D3DXVECTOR3( m_tDragData.vEndPos.x, m_tDragData.vStartPos.y, 0.f );

	for ( int i = 0; i < 4; ++i )
	{
		D3DXMatrixTranslation( &matTrans, vPoint[i].x, vPoint[i].y, vPoint[i].z );
		if ( i % 2 )
			D3DXMatrixScaling( &matScale, 1.f, m_tDragData.vEndPos.y - m_tDragData.vStartPos.y, 1.f );
		else
			D3DXMatrixScaling( &matScale, m_tDragData.vEndPos.x - m_tDragData.vStartPos.x, 1.f, 1.f );

		matWorld = matScale * matTrans;	
		CDevice::GetInstance()->GetSprite()->SetTransform( &matWorld );
		CDevice::GetInstance()->GetSprite()->Draw(
			m_pDragTexture->pTexture,
			NULL,
			NULL,
			NULL,
			D3DCOLOR_ARGB( 255, 255, 255, 255 )
		);
	}
}
