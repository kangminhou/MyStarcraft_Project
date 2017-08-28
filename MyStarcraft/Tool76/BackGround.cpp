#include "StdAfx.h"
#include "BackGround.h"
#include "Device.h"
#include "TextureMgr.h"
#include "tool76view.h"

void CBackGround::SetScroll( int iScrollX, int iScrollY )
{
	m_pMainView->SetScrollPos( 0, iScrollX );
	m_pMainView->SetScrollPos( 1, iScrollY );
}

int CBackGround::GetTileIndex(const D3DXVECTOR3& vPos)
{
	//타일 검사
	for(size_t i = 0; i < m_vecTile.size(); ++i)
	{
		if(Picking(vPos, i) )
		{
			return i;
		}
	}

	return -1;
}

void CBackGround::TileChange(const D3DXVECTOR3& vMousePos
							 , const int& iOption, const int& iHeight)
{
	int iTileIndex = GetTileIndex(vMousePos);

	if(iTileIndex == -1)
		return;

	m_vecTile[iTileIndex]->byOption = iOption;		//갈수 없다.
	m_vecTile[iTileIndex]->byHeight = iHeight;
	
	//if ( iOption == 0 )
	//	m_vecTile[iTileIndex]->byDrawID = 0;
	//else
	//	m_vecTile[iTileIndex]->byDrawID = 1;
	m_vecTile[iTileIndex]->byDrawID = iOption;
}

bool CBackGround::Picking(const D3DXVECTOR3& vMousePos,
						  const int& iIndex)
{
	//타일의 위치벡터
	POINT ptIndex;
	ptIndex.x = (LONG)(vMousePos.x / TILECX);
	ptIndex.y = (LONG)(vMousePos.y / TILECY);

	if ( iIndex != ptIndex.x + ptIndex.y * TILEX )
		return false;
	
	//D3DXVECTOR3 vPoint[4] = 
	//{
	//	D3DXVECTOR3(m_vecTile[iIndex]->vPos.x,
	//				m_vecTile[iIndex]->vPos.y - TILECY / 2.f,
	//				0.f),
	//
	//	D3DXVECTOR3(m_vecTile[iIndex]->vPos.x + TILECX / 2.f,
	//				m_vecTile[iIndex]->vPos.y,
	//				0.f),
	//
	//	D3DXVECTOR3(m_vecTile[iIndex]->vPos.x,
	//				m_vecTile[iIndex]->vPos.y + TILECY / 2.f,
	//				0.f),
	//
	//	D3DXVECTOR3(m_vecTile[iIndex]->vPos.x - TILECX / 2.f,
	//				m_vecTile[iIndex]->vPos.y,
	//				0.f)
	//};
	//
	//
	////벡터의 뺄셈으로 방향벡터를 구하자.
	//D3DXVECTOR3 vDirection[4] = 
	//{
	//	vPoint[1] - vPoint[0],
	//	vPoint[2] - vPoint[1],
	//	vPoint[3] - vPoint[2],
	//	vPoint[0] - vPoint[3]
	//};
	//
	//
	////법선벡터를 만들어 보자.
	//D3DXVECTOR3 vNormal[4] = 
	//{
	//	D3DXVECTOR3(vDirection[0].y, 
	//				-vDirection[0].x,
	//				0.f),
	//
	//	D3DXVECTOR3(vDirection[1].y, 
	//	-vDirection[1].x,
	//	0.f),
	//
	//	D3DXVECTOR3(vDirection[2].y, 
	//	-vDirection[2].x,
	//	0.f),
	//
	//	D3DXVECTOR3(vDirection[3].y, 
	//	-vDirection[3].x,
	//	0.f)
	//};
	//
	//
	////단위벡터로 만들어 주자.
	//for(int i = 0; i < 4; ++i)
	//	D3DXVec3Normalize(&vNormal[i], &vNormal[i]);
	//
	//for(int i = 0; i < 4; ++i)
	//{
	//	D3DXVECTOR3 vTemp = vMousePos - vPoint[i];
	//
	//	D3DXVec3Normalize(&vTemp, &vTemp);
	//
	//	float fDotResult = D3DXVec3Dot(&vTemp, &vNormal[i]);
	//
	//	//내적의 결과가 양수면 타일의 외부이고.
	//	//				음수면 타일의 내부이다.
	//	
	//	if(fDotResult > 0.f)
	//		return false;
	//}

	return true;
}

HRESULT CBackGround::Initialize(void)
{
	//int iA = 20;
	//if(10 = iA)
	//{
	//}

	this->m_pSprite = CDevice::GetInstance()->GetSprite();
	this->m_pFont = CDevice::GetInstance()->GetFont();

	for(int i = 0; i < TILEY; ++i)	
	{
		for(int j = 0; j < TILEX; ++j)
		{
			TILE* pTile = new TILE;

			//float fX = j * TILECX + ((i % 2) * (TILECX / 2.f));
			//float fY = i * (TILECY * 0.5f);

			float fX = float(j * TILECX);
			float fY = float(i * TILECY);

			pTile->vPos = D3DXVECTOR3(fX, fY, 0.f);
			pTile->vSize = D3DXVECTOR3( (float)TILECX, (float)TILECY, 0.f );
			
			pTile->byOption = 0;
			pTile->byDrawID = 0;

			m_vecTile.push_back(pTile);
		}
	}

	return S_OK;
}

int CBackGround::Update(void)
{
	return 0;
}

void CBackGround::Render(void)
{
	D3DXMATRIX matTrans;

	TCHAR szIndexText[MIDDLE_STR] = L"";

	D3DXMatrixTranslation( &matTrans, 0.f, 0.f, 0.f );
	this->m_pSprite->SetTransform( &matTrans );

	const TEX_INFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture( L"BackGround" );

	if ( !pTexInfo )
		return;

	RECT rc = { (m_pMainView->GetScrollPos( 0 )), (m_pMainView->GetScrollPos( 1 )), WINCX, WINCY };
	rc.right += rc.left;
	rc.bottom += rc.top;

	this->m_pSprite->Draw( pTexInfo->pTexture, &rc, NULL, NULL, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

	int iStartY = (m_pMainView->GetScrollPos( 1 )) / TILECY;
	int iEndY = ((m_pMainView->GetScrollPos( 1 )) + WINCY) / TILECY;

	int iStartX = (m_pMainView->GetScrollPos( 0 )) / (TILECX);
	int iEndX = ((m_pMainView->GetScrollPos( 0 )) + WINCX) / (TILECX);

	for(int i = iStartY; i < iEndY + 1; ++i)
	{
		for(int j = iStartX; j < iEndX + 1; ++j)
		{
			int iIndex = i * TILEX + j;

			if ( i < 0 || i >= TILEY || j < 0 || j >= TILEX )
				continue;

			//if ( this->m_vecTile[iIndex]->byDrawID == 0 )
			//	continue;

			const TEX_INFO* pTileTex = 
				CTextureMgr::GetInstance()->GetTexture(L"Back",
														L"Tile",
														m_vecTile[iIndex]->byDrawID);
	
			D3DXMatrixTranslation(&matTrans
				, m_vecTile[iIndex]->vPos.x - m_pMainView->GetScrollPos(0)	//0 : x
				, m_vecTile[iIndex]->vPos.y - m_pMainView->GetScrollPos(1)	//1 : y
				, 0.f);
	
			CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
	
			this->m_pSprite->Draw(
				pTileTex->pTexture,
				NULL, 
				&D3DXVECTOR3(0.f, 0.f, 0.f),
				NULL,
				D3DCOLOR_ARGB(255, 255, 255, 255)
				);
	
			//##폰트 출력
			//wsprintf(szIndexText, L"%d", iIndex);
			//
			//CDevice::GetInstance()->GetFont()->DrawTextW(
			//	CDevice::GetInstance()->GetSprite(),
			//	szIndexText,
			//	lstrlen(szIndexText),
			//	NULL,
			//	NULL,
			//	D3DCOLOR_ARGB(255, 0, 0, 255)
			//	);
		}
	}
}

void CBackGround::Release(void)
{
	for(size_t i = 0; i < m_vecTile.size(); ++i)
	{
		safe_delete(m_vecTile[i]);
	}
	m_vecTile.clear();
}

void CBackGround::MiniMapRender(void)
{
	D3DXMATRIX matTrans, matScale, matWorld;

	const TEX_INFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture( L"BackGround" );

	if ( !pTexInfo )
		return;

	float fBackScaleX = float( WINCX ) / float( pTexInfo->ImageInfo.Width );
	float fBackScaleY = float( WINCY ) / float( pTexInfo->ImageInfo.Height );

	D3DXMatrixScaling( &matScale, fBackScaleX, fBackScaleY, 1.f );
	D3DXMatrixTranslation( &matTrans, 0.f, 0.f, 0.f );

	matWorld = matScale * matTrans;

	CDevice::GetInstance()->GetSprite()->SetTransform( &matWorld );

	CDevice::GetInstance()->GetSprite()->Draw( pTexInfo->pTexture, NULL, NULL, NULL, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );


	const TEX_INFO* pMiniMapAreaTex = CTextureMgr::GetInstance()->GetTexture( L"MinimapArea" );

	float fMinimapScaleX = WINCX * fBackScaleX / pMiniMapAreaTex->ImageInfo.Width;
	float fMinimapScaleY = WINCY * fBackScaleY / pMiniMapAreaTex->ImageInfo.Height;
	D3DXMatrixScaling( &matScale, fMinimapScaleX, fMinimapScaleY, 1.f );
	D3DXMatrixTranslation( &matTrans, m_pMainView->GetScrollPos( 0 ) * fBackScaleX, m_pMainView->GetScrollPos( 1 ) * fBackScaleY, 0.f );

	matWorld = matScale * matTrans;

	CDevice::GetInstance()->GetSprite()->SetTransform( &matWorld );

	CDevice::GetInstance()->GetSprite()->Draw( pMiniMapAreaTex->pTexture, NULL, NULL, NULL, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

}

CBackGround::CBackGround(void)
{
}

CBackGround::~CBackGround(void)
{
	Release();
}
