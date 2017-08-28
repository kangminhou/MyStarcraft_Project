#include "stdafx.h"
#include "Mineral.h"

#include "TextureMgr.h"
#include "ObjMgr.h"

#include "Background.h"


CMineral::CMineral()
	: m_iMaxGatherNum(1)
{
}


CMineral::~CMineral()
{
}

void CMineral::SetUseEntity( CGameEntity * _pEntity )
{
	this->m_vecUseEntity.push_back( _pEntity );
}

void CMineral::SetMaxGatherNum( const int & _iMaxGatherNum )
{
	this->m_iMaxGatherNum = _iMaxGatherNum;
}

bool CMineral::GetIsCanGather() const
{
	return (this->m_vecUseEntity.size() < m_iMaxGatherNum);
}

int CMineral::GetMaxGatherNum() const
{
	return this->m_iMaxGatherNum;
}

HRESULT CMineral::Initialize( void )
{
	for ( size_t i = 0; i < 4; ++i )
	{
		this->m_vecMineralTexture[0].push_back( CTextureMgr::GetInstance()->GetTexture( L"Mineral", L"Trunk", i + 4 * 0 ) );
		this->m_vecMineralTexture[1].push_back( CTextureMgr::GetInstance()->GetTexture( L"Mineral", L"Trunk", i + 4 * 1 ) );
		this->m_vecMineralTexture[2].push_back( CTextureMgr::GetInstance()->GetTexture( L"Mineral", L"Trunk", i + 4 * 2 ) );

		this->m_vecMineralShadTexture[0].push_back( CTextureMgr::GetInstance()->GetTexture( L"Mineral", L"Shad", i + 4 * 0 ) );
		this->m_vecMineralShadTexture[1].push_back( CTextureMgr::GetInstance()->GetTexture( L"Mineral", L"Shad", i + 4 * 1 ) );
		this->m_vecMineralShadTexture[2].push_back( CTextureMgr::GetInstance()->GetTexture( L"Mineral", L"Shad", i + 4 * 2 ) );
	}

	this->m_iSelectMineral = 0;
	this->m_iCurMineralIndex = 0;

	float fShadStartX = (this->m_vecMineralShadTexture[0][0]->ImageInfo.Width - this->m_vecMineralTexture[0][0]->ImageInfo.Width) * 0.5f;
	float fShadStartY = (this->m_vecMineralShadTexture[0][0]->ImageInfo.Height - this->m_vecMineralTexture[0][0]->ImageInfo.Height) * 0.5f;

	D3DXMatrixTranslation( &m_shadWorldMatrix, fShadStartX, fShadStartY, 0.f );

	RECT rcTemp = { -40, -32, 40, 24 };
	this->m_tColRect = this->m_tOriginColRect = rcTemp;

	CGameEntity::Initialize();

	this->m_pBackground = this->m_pObjMgr->FindGameObject<CBackground>();

	return S_OK;
}

int CMineral::Update( void )
{
	//this->SetPos( CMouse::GetInstance()->GetPos() );
	D3DXVECTOR3 vPos = this->GetPos();

	this->UpdateMatrix();

	this->m_tColRect.left = (LONG)(this->m_tOriginColRect.left + vPos.x);
	this->m_tColRect.right = (LONG)(this->m_tOriginColRect.right + vPos.x);
	this->m_tColRect.top = (LONG)(this->m_tOriginColRect.top + vPos.y);
	this->m_tColRect.bottom = (LONG)(this->m_tOriginColRect.bottom + vPos.y);

	return 0;
}

void CMineral::Render( void )
{
	const TEX_INFO* pTrunkTexture = this->m_vecMineralTexture[this->m_iSelectMineral][this->m_iCurMineralIndex];
	const TEX_INFO* pShadTexture = this->m_vecMineralShadTexture[this->m_iSelectMineral][this->m_iCurMineralIndex];

	if ( pShadTexture )
	{
		D3DXMATRIX matWorld = m_shadWorldMatrix * this->GetWorldMatrix();

		this->DrawTexture( pShadTexture, 
						   matWorld,
						   D3DXVECTOR3( pShadTexture->ImageInfo.Width * 0.5f, pShadTexture->ImageInfo.Height * 0.5f, 0.f ) );
	}

	if ( pTrunkTexture )
	{
		this->DrawTexture( pTrunkTexture, 
						   this->GetWorldMatrix(),
						   D3DXVECTOR3( pTrunkTexture->ImageInfo.Width * 0.5f, pTrunkTexture->ImageInfo.Height * 0.5f, 0.f ) );
	}

	//TCHAR str[128];
	//swprintf_s( str, L"%f, %f, %f", CMouse::GetInstance()->GetPos().x, CMouse::GetInstance()->GetPos().y, 0.f );
	//D3DXMATRIX matTrans;
	//D3DXMatrixTranslation( &matTrans, 400.f, 400.f, 0.f );
	//this->DrawFont( matTrans, str );
	//swprintf( str, L"%d, %d, %d, %d", this->m_tColRect.left, this->m_tColRect.top, this->m_tColRect.right, this->m_tColRect.bottom );
	//D3DXMatrixTranslation( &matTrans, 400.f, 150.f, 0.f );
	//this->DrawFont( matTrans, str );

	//this->DrawRect( this->m_tColRect );
}

void CMineral::Release( void )
{
}

void CMineral::UpdatePosition( const D3DXVECTOR3 & vPrevPos )
{
	this->UpdateMatrix();
}

void CMineral::SetPattern( const eGameEntityPattern & _ePatternKind, const bool & _bPrevPattern /*= FALSE*/ )
{
}

void CMineral::UpdateTileData()
{
	this->m_tColRect.left += 8;
	this->m_tColRect.top += 16;
	this->m_tColRect.right -= 8;
	this->m_tColRect.bottom -= 8;
	this->m_pBackground->ObjectDataUpdate( this, 4 );
	this->m_tColRect.left -= 8;
	this->m_tColRect.top -= 16;
	this->m_tColRect.right += 8;
	this->m_tColRect.bottom += 8;
}

void CMineral::InitAnimation()
{
}

void CMineral::InitPattern()
{
}
