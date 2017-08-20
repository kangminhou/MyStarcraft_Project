#include "stdafx.h"
#include "MyButton.h"

#include "TextureMgr.h"
#include "KeyMgr.h"
#include "Mouse.h"


CMyButton::CMyButton()
{
}


CMyButton::~CMyButton()
{
	Release();
}

void CMyButton::SetButtonData( BUTTON_DATA * _pButtonData )
{
	this->m_pButtonData = _pButtonData;

	this->m_pDrawTexture[0] = CTextureMgr::GetInstance()->GetTexture( L"CmdIcon", L"Icon", this->m_pButtonData->nIconFrame );
	this->m_pDrawTexture[1] = CTextureMgr::GetInstance()->GetTexture( L"CmdIcon", L"IconWhite", this->m_pButtonData->nIconFrame );
	this->m_pDrawTexture[2] = CTextureMgr::GetInstance()->GetTexture( L"CmdIcon", L"IconWire", this->m_pButtonData->nIconFrame );

	if ( this->m_pButtonData->bCanUse )
		this->m_iDrawIndex = 0;
	else
		this->m_iDrawIndex = 2;

	m_bCanUseButton = this->m_pButtonData->bCanUse;
}

const BUTTON_DATA * CMyButton::GetButtonData() const
{
	return this->m_pButtonData;
}

HRESULT CMyButton::Initialize()
{
	m_bClickButton = false;

	CGameObject::Initialize();

	RECT tRect = { 0, 0, 40, 40 };
	m_tColRect = tRect;

	return S_OK;
}

int CMyButton::Update()
{
	if ( !this->m_pButtonData->bCanUse )
		return 0;

	if ( !m_bCanUseButton )
	{
		this->m_iDrawIndex = 0;
	}

	if ( CKeyMgr::GetInstance()->GetKeyOnceDown( this->m_pButtonData->byShortCutKey ) )
	{
		return 2;
	}

	D3DXVECTOR3 vMousePos = CMouse::GetInstance()->GetPos();

	if ( CKeyMgr::GetInstance()->GetKeyOnceDown( VK_LBUTTON ) && !m_bClickButton )
	{
		D3DXVECTOR3 vPos = this->GetPos();
		RECT rcCheck = { m_tColRect.left + vPos.x, m_tColRect.top + vPos.y, m_tColRect.right + vPos.x, m_tColRect.bottom + vPos.y };

		if ( rcCheck.left <= vMousePos.x && rcCheck.right >= vMousePos.x &&
			 rcCheck.top <= vMousePos.y && rcCheck.bottom >= vMousePos.y )
		{
			this->m_bClickButton = true;
			this->m_iDrawIndex = 1;
		}
	}

	if ( CKeyMgr::GetInstance()->GetKeyUp( VK_LBUTTON ) && m_bClickButton )
	{
		D3DXVECTOR3 vPos = this->GetPos();
		RECT rcCheck = { m_tColRect.left + vPos.x, m_tColRect.top + vPos.y, m_tColRect.right + vPos.x, m_tColRect.bottom + vPos.y };

		if ( rcCheck.left <= vMousePos.x && rcCheck.right >= vMousePos.x &&
			 rcCheck.top <= vMousePos.y && rcCheck.bottom >= vMousePos.y )
		{
			this->m_bClickButton = false;
			this->m_iDrawIndex = 0;
			return 1;
		}
	}

	return 0;
}

void CMyButton::Render()
{
	//const TEX_INFO* pTexture = this->m_pDrawTexture[this->m_iDrawIndex];

	this->DrawTexture( this->m_pDrawTexture[this->m_iDrawIndex],
					   this->GetWorldMatrix() );
}

void CMyButton::Release( void )
{
}

void CMyButton::UpdatePosition( const D3DXVECTOR3 & vPrevPos )
{
	D3DXMATRIX matTrans, matScale;

	D3DXVECTOR3 vSize = this->GetTransform()->GetSize();
	D3DXVECTOR3 vPos = this->GetTransform()->GetPos();

	D3DXMatrixScaling( &matScale, vSize.x, vSize.y, vSize.z );
	D3DXMatrixTranslation( &matTrans, vPos.x, vPos.y, 0.f );

	this->SetMatrix(matScale * matTrans);
}
