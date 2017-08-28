#include "stdafx.h"
#include "Gas.h"

#include "TextureMgr.h"
#include "ObjMgr.h"

#include "Background.h"
#include "Refinery.h"


CGas::CGas()
	: m_bCanGather(false)
	, m_byGatherEntityNum(0)
{
}


CGas::~CGas()
{
}

bool CGas::GetIsCanGather() const
{
	return this->m_bCanGather;
}

HRESULT CGas::Initialize( void )
{
	for ( size_t i = 0; i < 1; ++i )
	{
		this->m_vecGasaTexture.push_back( CTextureMgr::GetInstance()->GetTexture( L"Gas", L"Trunk", i ) );

		this->m_vecGasShadTexture.push_back( CTextureMgr::GetInstance()->GetTexture( L"Gas", L"Shad", i ) );
	}

	this->m_iSelectGasIndex = 0;

	//float fShadStartX = (this->m_vecMineralShadTexture[0][0]->ImageInfo.Width - this->m_vecMineralTexture[0][0]->ImageInfo.Width) * 0.5f;
	//float fShadStartY = (this->m_vecMineralShadTexture[0][0]->ImageInfo.Height - this->m_vecMineralTexture[0][0]->ImageInfo.Height) * 0.5f;
	//
	//D3DXMatrixTranslation( &m_shadWorldMatrix, fShadStartX, fShadStartY, 0.f );

	RECT rcTemp = { -64, -20, 32, 32 };
	this->m_tColRect = this->m_tOriginColRect = rcTemp;

	CGameEntity::Initialize();

	this->m_pBackground = this->m_pObjMgr->FindGameObject<CBackground>();

	return S_OK;
}

int CGas::Update( void )
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

void CGas::Render( void )
{
	const TEX_INFO* pTrunkTexture = this->m_vecGasaTexture[this->m_iSelectGasIndex];
	const TEX_INFO* pShadTexture = this->m_vecGasShadTexture[this->m_iSelectGasIndex];

	if ( pShadTexture )
	{
		//D3DXMATRIX matWorld = m_shadWorldMatrix * this->GetWorldMatrix();
		D3DXMATRIX matWorld = this->GetWorldMatrix();
	
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
	//swprintf_s( str, L"%f, %f, %f", this->GetWorldMatrix()._41, this->GetWorldMatrix()._42, 0.f );
	//D3DXMATRIX matTrans;
	//D3DXMatrixTranslation( &matTrans, 400.f, 400.f, 0.f );
	//this->DrawFont( matTrans, str );
	//swprintf( str, L"%d, %d, %d, %d", this->m_tColRect.left, this->m_tColRect.top, this->m_tColRect.right, this->m_tColRect.bottom );
	//D3DXMatrixTranslation( &matTrans, 400.f, 150.f, 0.f );
	//this->DrawFont( matTrans, str );

	//this->DrawRect( this->m_tColRect );
}

void CGas::Release( void )
{
}

void CGas::BuildRefinery( CRefinery * _pBuilding )
{
	this->m_bCanGather = true;
	this->m_pRefinery = _pBuilding;
}

void CGas::UpdateTileData()
{
	this->m_pBackground->ObjectDataUpdate( this, 3 );
}

bool CGas::GatherGas()
{
	if ( this->m_byGatherEntityNum >= 1 )
		return false;

	++m_byGatherEntityNum;

	return true;
}

void CGas::GatherEnd()
{
	if ( this->m_byGatherEntityNum <= 0 )
		return;

	--this->m_byGatherEntityNum;
}

void CGas::UpdatePosition( const D3DXVECTOR3 & vPrevPos )
{
	this->UpdateMatrix();
}

void CGas::SetPattern( const eGameEntityPattern & _ePatternKind, const bool & _bPrevPattern /*= FALSE*/ )
{
}

void CGas::InitAnimation()
{
}

void CGas::InitPattern()
{
}
