#include "stdafx.h"
#include "FogUnit.h"

#include "TextureMgr.h"
#include "ObjMgr.h"
#include "KeyMgr.h"
#include "TimeMgr.h"
#include "Device.h"

#include "FogBackground.h"


CFogUnit::CFogUnit()
{
}


CFogUnit::~CFogUnit()
{
}

HRESULT CFogUnit::Initialize( void )
{
	m_pTexture = CTextureMgr::GetInstance()->GetTexture( L"Marine", L"Idle", 0 );

	this->m_pBackground = CObjMgr::GetInstance()->FindGameObject<CFogBackground>();

	m_iSight = 9;

	m_fSpeed = 150.f;

	m_bMove = false;
	m_bClick = false;

	RECT tRect = { -16,-16,16,16 };
	m_tRect = m_tColRect = tRect;

	return S_OK;
}

int CFogUnit::Update( void )
{
	if ( this->m_bMove )
	{
		D3DXVECTOR3 vMove = m_vDestination - this->GetPos();

		D3DXVec3Normalize( &vMove, &vMove );

		this->Translate( vMove * m_fSpeed * GET_TIME );

		if ( D3DXVec3Length( &(m_vDestination - this->GetPos()) ) <= m_fSpeed * GET_TIME * 2.f )
			m_bMove = false;

	}
	
	this->KeyCheck();

	this->Scroll();

	D3DXVECTOR3 vPos = this->GetPos();

	this->m_tColRect.left = this->m_tRect.left + vPos.x;
	this->m_tColRect.top = this->m_tRect.top + vPos.y;
	this->m_tColRect.right = this->m_tRect.right + vPos.x;
	this->m_tColRect.bottom = this->m_tRect.bottom + vPos.y;

	return 0;
}

void CFogUnit::Render( void )
{
	D3DXVECTOR3 vPos( this->m_pTexture->ImageInfo.Width * 0.5f, this->m_pTexture->ImageInfo.Height * 0.5f, 0.f );
	D3DXVECTOR3 vMyPos = this->GetPos();

	D3DXMATRIX mat;

	D3DXMatrixTranslation( &mat, vMyPos.x - m_vScroll.x, vMyPos.y - m_vScroll.y, 0.f );

	CDevice::GetInstance()->GetSprite()->SetTransform( &mat );

	CDevice::GetInstance()->GetSprite()->Draw(
		this->m_pTexture->pTexture, nullptr, &vPos, nullptr, D3DCOLOR_ARGB( 255, 255, 255, 255 )
	);
}

void CFogUnit::Release( void )
{
}

void CFogUnit::UpdatePosition( const D3DXVECTOR3& vPrevPos )
{

}

void CFogUnit::Scroll()
{
	//D3DXVECTOR3 vPos = this->GetPos();
	//float fSpeed = 50.f;
	//
	//if ( vPos.x - m_vScroll.x > WINCX * 0.5f + 50.f )
	//	m_vScroll.x += fSpeed * GET_TIME;
	//else if ( vPos.x - m_vScroll.x < WINCX * 0.5f - 50.f )
	//	m_vScroll.x -= fSpeed * GET_TIME;
	//
	//if ( vPos.y - m_vScroll.y > WINCY * 0.5f + 50.f )
	//	m_vScroll.y += fSpeed * GET_TIME;
	//else if ( vPos.y - m_vScroll.y < WINCY * 0.5f - 50.f )
	//	m_vScroll.y -= fSpeed * GET_TIME;
	//
	//if ( this->m_vScroll.x < 0.f )
	//	this->m_vScroll.x = 0.f;
	//
	//if ( this->m_vScroll.y < 0.f )
	//	this->m_vScroll.y = 0.f;
}

void CFogUnit::KeyCheck()
{
	D3DXVECTOR3 vMove( 0.f, 0.f, 0.f );
	bool bMove = false;

	if ( CKeyMgr::GetInstance()->GetKeyOnceDown( VK_RBUTTON ) && m_bClick )
	{
		POINT ptMouse;

		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		D3DXVECTOR3 vMousePos = D3DXVECTOR3( (FLOAT)ptMouse.x, (FLOAT)ptMouse.y, 0.f );
		vMousePos += m_vScroll;

		m_vDestination = vMousePos;
		this->m_bMove = true;
	}

	if ( CKeyMgr::GetInstance()->GetKeyOnceDown( VK_LBUTTON ) )
	{
		POINT ptMouse;

		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		ptMouse.x += m_vScroll.x;
		ptMouse.y += m_vScroll.y;

		if ( PtInRect( &m_tColRect, ptMouse ) )
		{
			m_bClick = true;
		}
		else
			m_bClick = false;
	}
}
