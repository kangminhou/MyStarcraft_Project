#include "StdAfx.h"
#include "Mouse.h"

#include "KeyMgr.h"
#include "TextureMgr.h"
#include "Device.h"
#include "ObjMgr.h"
#include "TimeMgr.h"

#include "GameObject.h"
#include "Animation.h"
#include "Player.h"


IMPLEMENT_SINGLETON(CMouse)

CMouse::CMouse()
	: m_bMouseClick(false)
	, m_wstrDetailStateKey(L"")
	, m_bMinimapClick(false)
	, m_pPlayer(NULL)
{
	//ShowCursor( FALSE );
}

CMouse::~CMouse()
{
	Release();
}

void CMouse::SetMinimapClick( const bool & _bMinimapClick )
{
	this->m_bMinimapClick = _bMinimapClick;
}

void CMouse::SetPlayer( CPlayer * _pPlayer )
{
	this->m_pPlayer = _pPlayer;
}

bool CMouse::GetMinimapClick() const
{
	return this->m_bMinimapClick;
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
	this->m_fWaitCheckStateTime = 0.5f;
	this->m_eState = State_Idle;

	this->m_wstrDetailStateKey = L"Green";

	this->m_pSprite = CDevice::GetInstance()->GetSprite();

	this->m_pObjMgr = CObjMgr::GetInstance();

	this->m_pAnimCom = new CAnimation;
	this->m_pAnimCom->Initialize();

	this->InitAnimation();

#ifndef _IMAGE_THREAD_LOADING
	this->InitTexture();
	this->DecideMouseTexture();
#else
	this->m_bWaitTextureLoading = true;
#endif

	this->m_vScrollMoveSpeed = D3DXVECTOR3( 700.f, 600.f, 0.f );

	return S_OK;
}

int CMouse::Update( void )
{
	this->UpdatePosition();

#ifdef _IMAGE_THREAD_LOADING
	if ( this->m_bWaitTextureLoading )
	{
		if ( CTextureMgr::GetInstance()->GetIsEndLoading() )
		{
			this->InitTexture();
			this->DecideMouseTexture();

			ShowCursor( FALSE );
			this->m_bWaitTextureLoading = false;
		}
		else
			return Event_None;
	}
#endif

	this->UpdateMouseEvent();

	this->ScrollMoveCheck();

	this->DecideMouseState();

	if ( this->m_pAnimCom )
		this->m_pAnimCom->UpdateAnim();

	return Event_None;
}

void CMouse::Render( void )
{
	if ( this->m_bWaitTextureLoading )
		return;

	switch ( m_eState )
	{
		case State_Drag:
			this->DragAreaRender();
			break;
	}

	if ( this->m_pAnimCom )
	{
		const FRAME* pFrame = this->m_pAnimCom->GetCurAnimation();
		if ( pFrame->fIndex < this->m_vecCurTexture.size() )
		{
			const TEX_INFO* pTexture = this->m_vecCurTexture[(unsigned int)pFrame->fIndex];

			D3DXVECTOR3 vCenter( pTexture->ImageInfo.Width * 0.5f, pTexture->ImageInfo.Height * 0.5f, 0.f );
			
			this->m_pSprite->SetTransform( &this->m_matWorld );

			this->m_pSprite->Draw( pTexture->pTexture, NULL, &vCenter, NULL, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
	}

	if ( m_bMouseClick )
	{

	}

}

void CMouse::Release( void )
{
	for ( int i = 0; i < State_End; ++i )
	{
		for ( auto& iter : m_mapAllTextureArr[i] )
		{
			iter.second.clear();
		}

		m_mapAllTextureArr[i].clear();
	}

	safe_delete( this->m_pAnimCom );
}

void CMouse::ActSelectTargetState()
{
	this->m_eState = State_SelectTarget;

	this->DecideMouseTexture();
}

void CMouse::ActIdleState()
{
	this->m_eState = State_Idle;

	this->DecideMouseTexture(); 
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

	D3DXMatrixTranslation( &this->m_matWorld, m_vPos.x, m_vPos.y, 0.f );
}

void CMouse::UpdateMouseEvent()
{
	if ( this->m_bMinimapClick )
		return;

	if ( CKeyMgr::GetInstance()->GetKeyOnceDown( VK_LBUTTON ) )
	{
		this->m_bMouseClick = true;
		this->m_tDragData.vStartPos = this->m_vPos;
	}
	if ( CKeyMgr::GetInstance()->GetKeyStayDown( VK_LBUTTON ) )
	{
		this->m_tDragData.vEndPos = this->m_vPos;
		if ( this->m_tDragData.vEndPos != this->m_tDragData.vStartPos && m_eState != State_Drag )
		{
			this->m_eState = State_Drag;
			this->DecideMouseTexture();
		}
	}
	if ( CKeyMgr::GetInstance()->GetKeyUp( VK_LBUTTON ) )
	{
		this->m_bMouseClick = false;
		this->CheckDragData();

		this->m_eState = State_Idle;
		this->DecideMouseTexture();
	}
}

void CMouse::ScrollMoveCheck()
{
	if ( !this->m_pPlayer )
		return;

	if ( this->m_vPos.x <= 10.f )
		this->m_pPlayer->SetScroll( CGameObject::GetScroll() - D3DXVECTOR3( m_vScrollMoveSpeed.x * GET_TIME, 0.f, 0.f ) );
	else if ( this->m_vPos.x >= WINCX - 10.f )
		this->m_pPlayer->SetScroll( CGameObject::GetScroll() + D3DXVECTOR3( m_vScrollMoveSpeed.x * GET_TIME, 0.f, 0.f ) );

	if(this->m_vPos.y <= 10.f )
		this->m_pPlayer->SetScroll( CGameObject::GetScroll() - D3DXVECTOR3( 0.f, m_vScrollMoveSpeed.y * GET_TIME, 0.f ) );
	else if(this->m_vPos.y >= WINCY - 10.f )
		this->m_pPlayer->SetScroll( CGameObject::GetScroll() + D3DXVECTOR3( 0.f, m_vScrollMoveSpeed.y * GET_TIME, 0.f ) );
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

void CMouse::DecideMouseState()
{
	if ( this->m_eState == State_Scroll || this->m_eState == State_SelectTarget || this->m_eState == State_Drag )
		return;

	if ( m_fWaitCheckStateTime > 0.f )
	{
		m_fWaitCheckStateTime -= GET_TIME;
		return;
	}

	eMouseState eTempState = this->m_eState;

	this->m_eState = State_Idle;

	if ( this->m_vPos.x >= 0.f && this->m_vPos.x <= WINCX &&
		 this->m_vPos.y >= 0.f && this->m_vPos.y <= WINCY )
	{
		vector<CGameEntity*> vecEntity;

		this->m_tCheckStateDragData.vStartPos = this->m_tCheckStateDragData.vEndPos = this->m_vPos + CGameObject::GetScroll();

		for ( int i = OBJ_TYPE_USER; i <= OBJ_TYPE_USER2; ++i )
		{
			if ( m_pObjMgr->CheckDragEntitys( vecEntity, this->m_tCheckStateDragData, eObjectType( i ) ) )
			{
				this->m_wstrDetailStateKey = ((i == OBJ_TYPE_USER) ? L"Green" : L"Red");
				this->m_eState = State_HoverUnit;
				break;
			}
		}
	}

	if ( eTempState == this->m_eState )
		return;

	this->DecideMouseTexture();
}

void CMouse::DecideMouseTexture()
{
	if ( this->m_eState < State_Idle || this->m_eState >= State_End )
	{
		ERROR_MSG( L"CMouse DecideMouseTexture Failed ( Mouse State is Weird )" );
		return;
	}

	auto& iter = this->m_mapAllTextureArr[m_eState];

	auto& iterFind = iter.find( m_wstrDetailStateKey );

	if ( iterFind == iter.end() )
	{
		iterFind = iter.find( L"Green" );

		if ( iterFind == iter.end() )
		{
			ERROR_MSG( L"CMouse DecideMouseTexture Failed ( Unable to Find Mouse Texture )" );
			return;
		}
	}

	this->m_vecCurTexture = iterFind->second;

	switch ( m_eState )
	{
		case CMouse::State_Idle:
			this->m_pAnimCom->ChangeAnimation( L"Idle" );
			break;

		case CMouse::State_Drag:
			this->m_pAnimCom->ChangeAnimation( L"Drag" );
			break;

		case CMouse::State_HoverUnit:
			this->m_pAnimCom->ChangeAnimation( L"hover" );
			break;

		case CMouse::State_Scroll:
			this->m_pAnimCom->ChangeAnimation( L"Scroll" );
			break;

		case CMouse::State_SelectTarget:
			this->m_pAnimCom->ChangeAnimation( L"SelectTarget" );
			break;

	}
}

void CMouse::InitTexture()
{
	/* Get Drag Texture.. */
	m_pDragTexture = CTextureMgr::GetInstance()->GetTexture( L"Drag" );

	/* Get Mouse Idle State Texture.. */
	vector<const TEX_INFO*> vecTextureGreen;
	vector<const TEX_INFO*> vecTextureRed;
	vector<const TEX_INFO*> vecTextureYellow;

	for ( int i = 0; i < 5; ++i )
	{
		const TEX_INFO* pTexture = CTextureMgr::GetInstance()->GetTexture( L"Cursor", L"Arrow", i );
		vecTextureGreen.push_back( pTexture );
	}

	m_mapAllTextureArr[State_Idle].insert( make_pair( L"Green", vecTextureGreen ) );


	/* Get Mouse Drag State Texture.. */
	vecTextureGreen.clear();
	for ( int i = 0; i < 4; ++i )
	{
		const TEX_INFO* pTexture = CTextureMgr::GetInstance()->GetTexture( L"Cursor", L"Drag", i );
		vecTextureGreen.push_back( pTexture );
	}

	m_mapAllTextureArr[State_Drag].insert( make_pair( L"Green", vecTextureGreen ) );


	/* Get Mouse Hover State Texture.. */
	vecTextureGreen.clear();
	for ( int i = 0; i < 14; ++i )
	{
		const TEX_INFO* pTexture = CTextureMgr::GetInstance()->GetTexture( L"Cursor", L"magg", i );
		vecTextureGreen.push_back( pTexture );

		pTexture = CTextureMgr::GetInstance()->GetTexture( L"Cursor", L"magr", i );
		vecTextureRed.push_back( pTexture );

		pTexture = CTextureMgr::GetInstance()->GetTexture( L"Cursor", L"magy", i );
		vecTextureYellow.push_back( pTexture );
	}

	m_mapAllTextureArr[State_HoverUnit].insert( make_pair( L"Green", vecTextureGreen ) );
	m_mapAllTextureArr[State_HoverUnit].insert( make_pair( L"Red", vecTextureRed ) );
	m_mapAllTextureArr[State_HoverUnit].insert( make_pair( L"Yellow", vecTextureYellow ) );


	/* Get Mouse Scroll State Texture.. */
	vecTextureGreen.clear();
	vecTextureRed.clear();
	vecTextureYellow.clear();
	for ( int i = 0; i < 8; ++i )
	{
		const TEX_INFO* pTexture = CTextureMgr::GetInstance()->GetTexture( L"Cursor", L"Highscroll", i );
		vecTextureGreen.push_back( pTexture );

		pTexture = CTextureMgr::GetInstance()->GetTexture( L"Cursor", L"Lowscroll", i );
		vecTextureGreen.push_back( pTexture );
	}
	m_mapAllTextureArr[State_HoverUnit].insert( make_pair( L"Green", vecTextureGreen ) );


	/* Get Mouse SelectTarget State Texture.. */
	vecTextureGreen.clear();
	for ( int i = 0; i < 2; ++i )
	{
		const TEX_INFO* pTexture = CTextureMgr::GetInstance()->GetTexture( L"Cursor", L"Targg", i );
		vecTextureGreen.push_back( pTexture );

		pTexture = CTextureMgr::GetInstance()->GetTexture( L"Cursor", L"Targr", i );
		vecTextureRed.push_back( pTexture );

		pTexture = CTextureMgr::GetInstance()->GetTexture( L"Cursor", L"Targy", i );
		vecTextureYellow.push_back( pTexture );
	}

	m_mapAllTextureArr[State_SelectTarget].insert( make_pair( L"Green", vecTextureGreen ) );
	m_mapAllTextureArr[State_SelectTarget].insert( make_pair( L"Red", vecTextureRed ) );
	m_mapAllTextureArr[State_SelectTarget].insert( make_pair( L"Yellow", vecTextureYellow ) );

}

void CMouse::InitAnimation()
{
	this->m_pAnimCom->AddAnimation( L"Idle", FRAME( 0.f, 5.f, 5.f, 0.f ), CAnimation::Anim_Loop );
	this->m_pAnimCom->AddAnimation( L"Drag", FRAME( 0.f, 4.f, 4.f, 0.f ), CAnimation::Anim_Loop );
	this->m_pAnimCom->AddAnimation( L"Hover", FRAME( 0.f, 14.f, 14.f, 0.f ), CAnimation::Anim_Loop );
	this->m_pAnimCom->AddAnimation( L"Scroll", FRAME( 0.f, 2.f, 2.f, 0.f ), CAnimation::Anim_Loop );
	this->m_pAnimCom->AddAnimation( L"SelectTarget", FRAME( 0.f, 2.f, 2.f, 0.f ), CAnimation::Anim_Loop );
}
