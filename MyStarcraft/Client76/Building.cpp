#include "stdafx.h"
#include "Building.h"

#include "TextureMgr.h"
#include "ObjMgr.h"

#include "Animation.h"
#include "EntityPattern.h"
#include "Background.h"
#include "Player.h"

const TEX_INFO* CBuilding::m_pBasicBuildTextureArr[3][3];

CBuilding::CBuilding()
	: m_vTileOccupy( 0.f, 0.f, 0.f )
	, m_bSuccessBuild(false)
	, m_bApplyCol(false)
{
	this->SetObjKey( L"Building" );
}


CBuilding::~CBuilding()
{
}

void CBuilding::SetApplyCol( const bool & _bApplyCol )
{
	this->m_bApplyCol = _bApplyCol;
	this->m_pBackground->BuildingDataUpdate( this );
	CObjMgr::GetInstance()->EraseEntitySpaceData( this, this->m_iEntitySpaceDataKey );
	CObjMgr::GetInstance()->InsertEntitySpaceData( this );
}

HRESULT CBuilding::Initialize( void )
{
	if ( this->GetObjectType() == OBJ_TYPE_USER )
		this->m_pPlayer = (CPlayer*)CObjMgr::GetInstance()->GetList( this->GetObjectType() )->front();

	this->m_vecEntityInformation.push_back( CGameEntity::Entity_Building );

	InitTexture();
	CGameEntity::Initialize();

	return S_OK;
}

int CBuilding::Update( void )
{
	if ( m_bDie )
	{
		if ( m_bDestoryEntity )
			return Event_DestoryObject;
		else
		{
			if ( m_pAnimCom->GetIsAnimEnd() )
			{
				m_bDestoryEntity = true;
				return Event_DestoryObject;
			}
		}
	}

	if ( this->m_bScrollMove )
		this->UpdateMatrix();

	if ( this->m_pCurActPattern )
		this->m_pCurActPattern->Update();

	if ( this->m_pAnimCom )
		this->m_pAnimCom->UpdateAnim();

	this->DecideTexture();

	D3DXVECTOR3 vPos = this->GetPos();
	this->m_tColRect.left = this->m_tOriginColRect.left + vPos.x;
	this->m_tColRect.right = this->m_tOriginColRect.right + vPos.x;
	this->m_tColRect.top = this->m_tOriginColRect.top + vPos.y;
	this->m_tColRect.bottom = this->m_tOriginColRect.bottom + vPos.y;

	return Event_None;
}

void CBuilding::FrameRender( const D3DXMATRIX & _matWorld )
{
	const TEX_INFO* pTexture = ((this->m_mapAllTexture.find( L"Build" )->second.empty()) ? NULL : this->m_mapAllTexture.find( L"Build" )->second.back());

	if ( pTexture )
	{
		/* 그림이 중앙이 객체의 좌표가 되도록 설정.. */
		float fX = pTexture->ImageInfo.Width * 0.5f;
		float fY = pTexture->ImageInfo.Height * 0.5f;
		this->DrawTexture( pTexture, _matWorld, D3DXVECTOR3( fX, fY, 0.f ) );
	}
}

void CBuilding::UpdateLookAnimIndex()
{
}

void CBuilding::SuccessBuild()
{
	CEntityMgr::GetInstance()->BuildBuilding( this->m_eEntityType );
}

D3DXVECTOR3 CBuilding::CalcNearEmptySpace()
{
	D3DXVECTOR3 vOut = this->GetPos();
	int iDist = 0;
	int iStartTileIndex = this->m_pBackground->GetTileIndex( vOut );

	//int iStartTileIndexX = iStartTileIndex % TILEX;
	//int iStartTileIndexY = iStartTileIndex / TILEX;

	int iStartTileIndexY = (iStartTileIndex / TILEX);
	int iStartTileIndexX = (iStartTileIndex - iStartTileIndexY * TILEX);

	while ( true )
	{
		++iDist;

		for ( int i = 0; i < 4; ++i )
		{
			for ( int j = -iDist; j < iDist; ++j )
			{
				int iStartX = 0;
				int iStartY = 0;

				switch ( i )
				{
					case 0:
						iStartX = (iStartTileIndexX + j);
						iStartY = (iStartTileIndexY + iDist);
						break;

					case 1:
						iStartX = (iStartTileIndexX + iDist);
						iStartY = (iStartTileIndexY - j);
						break;

					case 2:
						iStartX = (iStartTileIndexX - j);
						iStartY = (iStartTileIndexY - iDist);
						break;

					case 3:
						iStartX = (iStartTileIndexX - iDist);
						iStartY = (iStartTileIndexY + j);
						break;

				}

				if ( iStartX < 0 || iStartX >= TILEX ||
					 iStartY < 0 || iStartY >= TILEY )
					continue;

				int iIndex = iStartX + iStartY * TILEX;

				if ( this->m_pBackground->CheckCanGoTile( iIndex, 5, NULL, true ) )
				{
					vOut = D3DXVECTOR3( TILECX * iStartX + TILECX * 0.5f, TILECY * iStartY + TILECY * 0.5f, 0.f );
					//vOut = D3DXVECTOR3( TILECX * iStartX, TILECY * iStartY, 0.f );
					cout << "x : " << vOut.x << ", y : " << vOut.y << ", z : " << vOut.z << endl;
					cout << "iStartTileIndexX : " << iStartTileIndexX << ", iStartTileIndexY : " << iStartTileIndexY << endl;
					cout << "iStartX : " << iStartX << ", iStartY : " << iStartY << endl;
					return vOut;
				}
			}
		}
	}

	return vOut;
}

void CBuilding::InitBasicBuildTexture()
{
	for ( int i = 0; i < 3; ++i )
	{
		m_pBasicBuildTextureArr[0][i] = CTextureMgr::GetInstance()->GetTexture( L"Building", L"Tbldlrg(96,96)1", i );
		m_pBasicBuildTextureArr[1][i] = CTextureMgr::GetInstance()->GetTexture( L"Building", L"Tbldlrg(96,96)2", i );
		m_pBasicBuildTextureArr[2][i] = CTextureMgr::GetInstance()->GetTexture( L"Building", L"Tbldlrg(160,128)", i );
	}
}
