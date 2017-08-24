#include "stdafx.h"
#include "Building.h"

#include "TextureMgr.h"
#include "ObjMgr.h"

#include "Animation.h"
#include "EntityPattern.h"
#include "Background.h"
#include "Player.h"
#include "MiniMap.h"

const TEX_INFO* CBuilding::m_pBasicBuildTextureArr[3][3];

CBuilding::CBuilding()
	: m_vTileOccupy( 0.f, 0.f, 0.f )
	, m_bSuccessBuild(false)
	, m_bApplyCol(false)
	, m_bCanBuild(false)
{
	this->SetObjKey( L"Building" );
}


CBuilding::~CBuilding()
{
}

void CBuilding::SetApplyCol( const bool & _bApplyCol )
{
	this->m_bApplyCol = _bApplyCol;
	this->m_pBackground->ObjectDataUpdate( this );
	CObjMgr::GetInstance()->EraseEntitySpaceData( this, this->m_iEntitySpaceDataKey );
	CObjMgr::GetInstance()->InsertEntitySpaceData( this );
}

bool CBuilding::GetIsSuccessBuild() const
{
	return this->m_bSuccessBuild;
}

bool CBuilding::GetIsCanBuild() const
{
	return this->m_bCanBuild;
}

HRESULT CBuilding::Initialize( void )
{
	if ( this->GetObjectType() == OBJ_TYPE_USER )
		this->m_pPlayer = (CPlayer*)CObjMgr::GetInstance()->GetList( this->GetObjectType() )->front();

	this->m_vecEntityInformation.push_back( CGameEntity::Entity_Building );

	this->m_pBuildRectTexture[0] = CTextureMgr::GetInstance()->GetTexture( L"Build_OK" );
	this->m_pBuildRectTexture[1] = CTextureMgr::GetInstance()->GetTexture( L"Build_OFF" );

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
	this->m_tColRect.left = (LONG)(this->m_tOriginColRect.left + vPos.x);
	this->m_tColRect.right = (LONG)(this->m_tOriginColRect.right + vPos.x);
	this->m_tColRect.top = (LONG)(this->m_tOriginColRect.top + vPos.y);
	this->m_tColRect.bottom = (LONG)(this->m_tOriginColRect.bottom + vPos.y);

	return Event_None;
}

void CBuilding::Render( void )
{
	/* �׸��� �߾��� ��ü�� ��ǥ�� �ǵ��� ����.. */
	if ( m_pCurDrawTexture )
	{
		float fX = m_pCurDrawTexture->ImageInfo.Width * 0.5f;
		float fY = m_pCurDrawTexture->ImageInfo.Height * 0.5f;
		this->DrawTexture( m_pCurDrawTexture, this->GetWorldMatrix(), D3DXVECTOR3( fX, fY, 0.f ) );
	}

	D3DXMATRIX matTrans;
	D3DXMatrixTranslation( &matTrans, 500.f, 400.f, 0.f );
	TCHAR str[128];
	swprintf_s( str, L"%f, %f", this->m_tInfoData.fMaxHp, this->m_tInfoData.fCurHp );
	this->DrawFont( matTrans, str );

	//this->DrawRect( this->m_tColRect );

	if ( this->m_bDrawHpBarUI )
	{
		this->RenderHpUI();
	}
}

void CBuilding::FrameRender( const D3DXMATRIX & _matWorld )
{
	const TEX_INFO* pTexture = ((this->m_mapAllTexture.find( L"Build" )->second.empty()) ? NULL : this->m_mapAllTexture.find( L"Build" )->second.back());

	if ( pTexture )
	{
		/* �׸��� �߾��� ��ü�� ��ǥ�� �ǵ��� ����.. */
		float fX = pTexture->ImageInfo.Width * 0.5f;
		float fY = pTexture->ImageInfo.Height * 0.5f;
		this->DrawTexture( pTexture, _matWorld, D3DXVECTOR3( fX, fY, 0.f ) );
	}
}

void CBuilding::RectRender( const RECT & _rcDraw )
{
	int iStartIndex = this->m_pBackground->GetTileIndex( D3DXVECTOR3( (float)_rcDraw.left, (float)_rcDraw.top, 0.f ) );
	int iEndIndex = this->m_pBackground->GetTileIndex( D3DXVECTOR3( (float)_rcDraw.right, (float)_rcDraw.bottom, 0.f ) );

	int iStartX = iStartIndex % TILEX;
	int iStartY = iStartIndex / TILEX;
	int iEndX = iEndIndex % TILEX;
	int iEndY = iEndIndex / TILEX;

	D3DXMATRIX matWorld;
	D3DXVECTOR3 vCenter( m_pBuildRectTexture[0]->ImageInfo.Width * 0.5f, m_pBuildRectTexture[0]->ImageInfo.Height * 0.5f, 0.f );

	this->m_bCanBuild = true;

	for ( int i = iStartY; i < iEndY; ++i )
	{
		for ( int j = iStartX; j < iEndX; ++j )
		{
			D3DXMatrixTranslation( &matWorld, j * TILECX + vCenter.x, i * TILECY + vCenter.y, 0.f );
			int iIndex = j + i * TILEX;
			if ( this->CheckCanBuild( iIndex ) )
			{
				this->DrawTexture( this->m_pBuildRectTexture[0], matWorld, vCenter );
			}
			else
			{
				if ( this->m_bCanBuild )
					this->m_bCanBuild = false;
				this->DrawTexture( this->m_pBuildRectTexture[1], matWorld, vCenter );
			}
		}
	}
}

void CBuilding::BuildStart()
{
	this->m_iMinimapSpaceDataKey = this->m_pMiniMap->MoveEntity( this );
}

void CBuilding::UpdateLookAnimIndex()
{
}

void CBuilding::SuccessBuild()
{
	CObjMgr::GetInstance()->EraseEntitySpaceData( this, this->m_iEntitySpaceDataKey );
	CObjMgr::GetInstance()->InsertEntitySpaceData( this );

	vector<CGameEntity*> vecEntity;
	CObjMgr::GetInstance()->CheckEntitysCol( &vecEntity, this, this->GetObjectType() );

	D3DXVECTOR3 vBuildingPos = this->GetPos();
	int iColRectCYHalf = ((this->m_tColRect.bottom - this->m_tColRect.top) / 2);

	for ( size_t i = 0; i < vecEntity.size(); ++i )
	{
		D3DXVECTOR3 vCheckEntityPos = vecEntity[i]->GetPos();
		RECT rcCheckEntityColRect = vecEntity[i]->GetColRect();

		if ( vBuildingPos.y >= vCheckEntityPos.y )
			vCheckEntityPos.y = vBuildingPos.y - iColRectCYHalf - ((rcCheckEntityColRect.bottom - rcCheckEntityColRect.top) / 2);
		else
			vCheckEntityPos.y = vBuildingPos.y + iColRectCYHalf + ((rcCheckEntityColRect.bottom - rcCheckEntityColRect.top) / 2);

		vecEntity[i]->SetPos( vCheckEntityPos );

	}

	m_bSuccessBuild = true;
	this->DecideTexture();
	CEntityMgr::GetInstance()->BuildBuilding( this->m_eEntityType );
}

bool CBuilding::CheckCanBuild( const int & _iIndex )
{
	return this->m_pBackground->CheckCanGoTile(_iIndex, 8, this, true);
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
