#include "stdafx.h"
#include "StarcraftStartScene.h"

#include "KeyMgr.h"
#include "SceneMgr.h"
#include "Mouse.h"
#include "TimeMgr.h"
#include "Device.h"
#include "TextureMgr.h"
#include "SceneMgr.h"


CStarcraftStartScene::CStarcraftStartScene()
{
}


CStarcraftStartScene::~CStarcraftStartScene()
{
}

HRESULT CStarcraftStartScene::Initialize( void )
{
	this->m_pKeyMgr = CKeyMgr::GetInstance();
	this->m_pMouse = CMouse::GetInstance();
	this->m_pTimeMgr = CTimeMgr::GetInstance();
	this->m_pSprite = CDevice::GetInstance()->GetSprite();
	this->m_pSceneMgr = CSceneMgr::GetInstance();

	this->m_iHoverButton = -1;

	for ( int i = 0; i < 4; ++i )
	{
		ZeroMemory( &this->m_tButton[i], sizeof( BUTTON ) );
	}

	D3DXMatrixTranslation( &this->m_tButton[0].matWorld, 100.f, 100.f, 0.f );

	for ( int i = 0; i < 35; ++i )
		this->m_tButton[0].vecTexture.push_back( CTextureMgr::GetInstance()->GetTexture( L"MainMenu", L"SinglePlay", i ) );

	for ( int i = 0; i < 60; ++i )
		this->m_tButton[0].vecHoverTexture.push_back( CTextureMgr::GetInstance()->GetTexture( L"MainMenu", L"SMouseOn", i ) );

	for ( int i = 0; i < 85; ++i )
		this->m_tButton[1].vecTexture.push_back( CTextureMgr::GetInstance()->GetTexture( L"MainMenu", L"Editor", i ) );

	for ( int i = 0; i < 20; ++i )
		this->m_tButton[1].vecHoverTexture.push_back( CTextureMgr::GetInstance()->GetTexture( L"MainMenu", L"EMouseOn", i ) );

	for ( int i = 0; i < 50; ++i )
		this->m_tButton[3].vecTexture.push_back( CTextureMgr::GetInstance()->GetTexture( L"MainMenu", L"Exit", i ) );

	for ( int i = 0; i < 30; ++i )
		this->m_tButton[3].vecHoverTexture.push_back( CTextureMgr::GetInstance()->GetTexture( L"MainMenu", L"XMouseOn", i ) );

	for ( int i = 0; i < 2; ++i )
	{
		this->m_tButton[0].pFontTexture[i] = CTextureMgr::GetInstance()->GetTexture( L"BUTTON", L"SINGLE", i );
		this->m_tButton[1].pFontTexture[i] = CTextureMgr::GetInstance()->GetTexture( L"BUTTON", L"EDIT", i );
		this->m_tButton[3].pFontTexture[i] = CTextureMgr::GetInstance()->GetTexture( L"BUTTON", L"EXIT", i );
	}
	
	D3DXVECTOR3 vStart( 200.f, 100.f, 0.f );
	D3DXVECTOR3 vButtonMatrixPos;

	for ( int i = 0; i < 4; ++i )
	{
		vButtonMatrixPos = D3DXVECTOR3( vStart.x + ((i % 2) * 350.f), 
										vStart.y + ((i / 2) * 250.f) + ((i % 2) * 100.f), 
										0.f );

		D3DXMatrixTranslation( &this->m_tButton[i].matWorld, vButtonMatrixPos.x, vButtonMatrixPos.y, 0.f );
	}

	D3DXMATRIX matTrans;
	vButtonMatrixPos = D3DXVECTOR3( this->m_tButton[0].matWorld._41, this->m_tButton[0].matWorld._42, 0.f );
	D3DXMatrixTranslation( &matTrans, 206.f, 172.f, 0.f );
	this->m_tButton[0].matHoverWorld = matTrans;
	
	vButtonMatrixPos = D3DXVECTOR3( this->m_tButton[1].matWorld._41, this->m_tButton[1].matWorld._42, 0.f );
	D3DXMatrixTranslation( &matTrans, 594.f, 214.f, 0.f );
	this->m_tButton[1].matHoverWorld = matTrans;

	//vButtonMatrixPos = D3DXVECTOR3( this->m_tButton[2].matWorld._41, this->m_tButton[2].matWorld._42, 0.f );
	//D3DXMatrixTranslation( &matTrans, 592.f - vButtonMatrixPos.x, 136.f - vButtonMatrixPos.y, 0.f );
	//this->m_tButton[2].matHoverWorld = matTrans * this->m_tButton[2].matWorld;
	
	vButtonMatrixPos = D3DXVECTOR3( this->m_tButton[3].matWorld._41, this->m_tButton[3].matWorld._42, 0.f );
	D3DXMatrixTranslation( &matTrans, 582.f, 450.f, 0.f );
	this->m_tButton[3].matHoverWorld = matTrans;

	this->m_tButton[0].rcClick.left = -120;
	this->m_tButton[0].rcClick.top = -80;
	this->m_tButton[0].rcClick.right = 150;
	this->m_tButton[0].rcClick.bottom = 80;

	this->m_tButton[1].rcClick.left = -60;
	this->m_tButton[1].rcClick.top = -100;
	this->m_tButton[1].rcClick.right = 70;
	this->m_tButton[1].rcClick.bottom = 120;

	this->m_tButton[2].rcClick.left = -50;
	this->m_tButton[2].rcClick.top = -50;
	this->m_tButton[2].rcClick.right = 50;
	this->m_tButton[2].rcClick.bottom = 50;

	this->m_tButton[3].rcClick.left = -100;
	this->m_tButton[3].rcClick.top = -100;
	this->m_tButton[3].rcClick.right = 100;
	this->m_tButton[3].rcClick.bottom = 100;

	D3DXMatrixTranslation( &this->m_tButton[0].matFontWorld, 259.f, 166.f, 0.f );
	D3DXMatrixTranslation( &this->m_tButton[1].matFontWorld, 565.f, 110.f, 0.f );
	//D3DXMatrixTranslation( &this->m_tButton[2].matFontWorld, 259, 166, 0.f );
	D3DXMatrixTranslation( &this->m_tButton[3].matFontWorld, 555.f, 358.f, 0.f );

	m_pBackTexture = CTextureMgr::GetInstance()->GetTexture( L"StartBack" );

	this->m_pDragTexture = CTextureMgr::GetInstance()->GetTexture( L"Drag" );

	return S_OK;
}

int CStarcraftStartScene::Update( void )
{
	D3DXVECTOR3 vMouse = this->m_pMouse->GetPos();
	POINT ptMouse = { vMouse.x,vMouse.y };

	if ( m_iHoverButton != -1 )
		this->m_tButton[m_iHoverButton].bSelect = false;

	for ( int i = 0; i < 4; ++i )
	{
		this->m_tButton[i].fCurFrame += this->m_pTimeMgr->GetTime() * 10.f;

		if ( (unsigned int)this->m_tButton[i].fCurFrame >= this->m_tButton[i].vecTexture.size() )
		{
			this->m_tButton[i].fCurFrame = 0.f;
		}

		D3DXVECTOR3 vButtonPos = D3DXVECTOR3( this->m_tButton[i].matWorld._41, this->m_tButton[i].matWorld._42, 0.f );
		RECT rcCol = { m_tButton[i].rcClick.left + vButtonPos.x, m_tButton[i].rcClick.top + vButtonPos.y,
			m_tButton[i].rcClick.right + vButtonPos.x, m_tButton[i].rcClick.bottom + vButtonPos.y };

		if ( PtInRect( &rcCol, ptMouse ) )
		{
			m_iHoverButton = i;
			this->m_tButton[m_iHoverButton].bSelect = true;
		}

	}

	if ( this->m_pKeyMgr->GetKeyOnceDown( VK_LBUTTON ) )
	{
		D3DXVECTOR3 vButtonPos = D3DXVECTOR3( this->m_tButton[0].matWorld._41, this->m_tButton[0].matWorld._42, 0.f );
		RECT rcCol = { m_tButton[0].rcClick.left + vButtonPos.x, m_tButton[0].rcClick.top + vButtonPos.y,
			m_tButton[0].rcClick.right + vButtonPos.x, m_tButton[0].rcClick.bottom + vButtonPos.y };

		D3DXVECTOR3 vMousePos = this->m_pMouse->GetPos();

		RECT rcMouse = { (LONG)(vMousePos.x) - 1, (LONG)(vMousePos.y) - 1, (LONG)(vMousePos.x) + 1, (LONG)(vMousePos.y) + 1 };
		RECT rc = { 0, 0, 0, 0 };

		if ( IntersectRect( &rc, &rcCol, &rcMouse ) )
		{
			this->m_pSceneMgr->SetChangeScene( SCENE_STAGE );
		}

	}

#ifdef STARCRAFT_START_SCENE_DEBUG
	if ( this->m_pKeyMgr->GetKeyOnceDown( VK_RETURN ) )
	{
		for ( int i = 0; i < 4; ++i )
		{
			D3DXVECTOR3 vPos = D3DXVECTOR3( this->m_tButton[i].matHoverWorld._41, this->m_tButton[i].matHoverWorld._42, 0.f );
			cout << "x : " << vPos.x << ", y : " << vPos.y << endl;
		}
	}

	if ( this->m_pKeyMgr->GetKeyStayDown( VK_CONTROL ) )
	{
		//D3DXMATRIX* pClickMatrix = nullptr;
		D3DXVECTOR3 vMousePos = this->m_pMouse->GetPos();
		
		if ( this->m_pKeyMgr->GetKeyStayDown( '1' ) )
		{
			//D3DXMatrixTranslation( &this->m_tButton[0].matFontWorld, vMousePos.x, vMousePos.y, 0.f );
			D3DXMatrixTranslation( &this->m_tButton[0].matHoverWorld, vMousePos.x, vMousePos.y, 0.f );
		}
		else if ( this->m_pKeyMgr->GetKeyStayDown( '2' ) )
		{
			//D3DXMatrixTranslation( &this->m_tButton[1].matFontWorld, vMousePos.x, vMousePos.y, 0.f );
			D3DXMatrixTranslation( &this->m_tButton[1].matHoverWorld, vMousePos.x, vMousePos.y, 0.f );
		}
		else if ( this->m_pKeyMgr->GetKeyStayDown( '3' ) )
		{
			//D3DXMatrixTranslation( &this->m_tButton[2].matFontWorld, vMousePos.x, vMousePos.y, 0.f );
			D3DXMatrixTranslation( &this->m_tButton[2].matHoverWorld, vMousePos.x, vMousePos.y, 0.f );
		}
		else if ( this->m_pKeyMgr->GetKeyStayDown( '4' ) )
		{
			//D3DXMatrixTranslation( &this->m_tButton[3].matFontWorld, vMousePos.x, vMousePos.y, 0.f );
			D3DXMatrixTranslation( &this->m_tButton[3].matHoverWorld, vMousePos.x, vMousePos.y, 0.f );
		}

		//D3DXMatrixTranslation( pClickMatrix, vMousePos.x, vMousePos.y, 0.f );
	}
#endif

	return 0;
}

void CStarcraftStartScene::Render( void )
{
	if ( !this->m_pSprite )
		return;

	D3DXMATRIX matTrans;
	D3DXMatrixTranslation( &matTrans, 0.f, 0.f, 0.f );
	this->m_pSprite->SetTransform( &matTrans );
	this->m_pSprite->Draw( this->m_pBackTexture->pTexture, nullptr, nullptr, nullptr, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

	const TEX_INFO* pTexture = nullptr;

	vector<pair<D3DXMATRIX, const TEX_INFO*>> vecDrawData;

	for ( int i = 0; i < 4; ++i )
	{
		if ( this->m_tButton[i].vecTexture.empty() )
			continue;

		pTexture = this->m_tButton[i].pFontTexture[((this->m_tButton[i].bSelect) ? 1 : 0)];
		vecDrawData.push_back( make_pair( this->m_tButton[i].matFontWorld, pTexture ) );

		if ( this->m_tButton[i].bSelect )
		{
			size_t iIndex = (unsigned int)(this->m_tButton[i].fCurFrame * 
				((float)this->m_tButton[i].vecHoverTexture.size() / (float)this->m_tButton[i].vecTexture.size()));

			if ( iIndex < this->m_tButton[i].vecHoverTexture.size() )
				pTexture = this->m_tButton[i].vecHoverTexture[iIndex];
			else
				pTexture = nullptr;

			vecDrawData.push_back( make_pair( this->m_tButton[i].matHoverWorld, pTexture ) );
		}

		pTexture = this->m_tButton[i].vecTexture[(unsigned int)this->m_tButton[i].fCurFrame];

		if ( this->m_tButton[i].bSelect && i == 0 )
			vecDrawData.insert( vecDrawData.begin() + 1, make_pair( this->m_tButton[i].matWorld, pTexture ) );
		else
			vecDrawData.push_back( make_pair( this->m_tButton[i].matWorld, pTexture ) );

		D3DXVECTOR3 vCenter( 0.f, 0.f, 0.f );

		for ( int i = vecDrawData.size() - 1; i >= 0; --i )
		{
			if ( !vecDrawData[i].second )
				continue;

			vCenter.x = vecDrawData[i].second->ImageInfo.Width * 0.5f;
			vCenter.y = vecDrawData[i].second->ImageInfo.Height * 0.5f;

			m_pSprite->SetTransform( &vecDrawData[i].first );
			m_pSprite->Draw( vecDrawData[i].second->pTexture, nullptr, &vCenter, nullptr, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}

#ifdef STARCRAFT_START_SCENE_DEBUG
		D3DXVECTOR3 vButtonPos = D3DXVECTOR3( this->m_tButton[i].matWorld._41, this->m_tButton[i].matWorld._42, 0.f );
		RECT rcCol = { m_tButton[i].rcClick.left + vButtonPos.x, m_tButton[i].rcClick.top + vButtonPos.y,
			m_tButton[i].rcClick.right + vButtonPos.x, m_tButton[i].rcClick.bottom + vButtonPos.y };
		
		this->DrawRect( rcCol );
#endif
	}

}

void CStarcraftStartScene::Release( void )
{
}

void CStarcraftStartScene::DrawRect( const RECT & _rc )
{
	D3DXMATRIX matTrans, matScale, matWorld;

	D3DXVECTOR3 vPoint[4];

	vPoint[0] = D3DXVECTOR3( FLOAT(_rc.left), FLOAT(_rc.top), 0.f );
	vPoint[1] = D3DXVECTOR3( FLOAT(_rc.left), FLOAT(_rc.top), 0.f );
	vPoint[2] = D3DXVECTOR3( FLOAT(_rc.left), FLOAT(_rc.bottom), 0.f );
	vPoint[3] = D3DXVECTOR3( FLOAT(_rc.right), FLOAT(_rc.top), 0.f );

	for ( int i = 0; i < 4; ++i )
	{
		D3DXMatrixTranslation( &matTrans, vPoint[i].x, vPoint[i].y, vPoint[i].z );
		if ( i % 2 )
			D3DXMatrixScaling( &matScale, 1.f, FLOAT(_rc.bottom - _rc.top), 1.f );
		else
			D3DXMatrixScaling( &matScale, FLOAT( _rc.right - _rc.left), 1.f, 1.f );

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
