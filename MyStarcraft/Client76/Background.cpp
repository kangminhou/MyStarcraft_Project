#include "stdafx.h"
#include "Background.h"

#include "Device.h"
#include "TextureMgr.h"


CBackground::CBackground()
{
}


CBackground::~CBackground()
{
	Release();
}

void CBackground::SetScroll( int iScrollX, int iScrollY )
{
	//m_pMainView->SetScrollPos( 0, iScrollX );
	//m_pMainView->SetScrollPos( 1, iScrollY );
}

int CBackground::GetTileIndex(const D3DXVECTOR3& vPos)
{
	//Ÿ�� �˻�
	for(size_t i = 0; i < m_vecTile.size(); ++i)
	{
		if(Picking(vPos, i) )
		{
			return i;
		}
	}

	return -1;
}

void CBackground::TileChange(const D3DXVECTOR3& vMousePos
							  , const int& iOption, const int& iHeight)
{
	int iTileIndex = GetTileIndex(vMousePos);

	if(iTileIndex == -1)
		return;

	m_vecTile[iTileIndex]->byOption = iOption;		//���� ����.
	m_vecTile[iTileIndex]->byHeight = iHeight;

	if ( iOption == 0 )
		m_vecTile[iTileIndex]->byDrawID = 0;
	else
		m_vecTile[iTileIndex]->byDrawID = 1;
}

bool CBackground::Picking(const D3DXVECTOR3& vMousePos,
						   const int& iIndex)
{
	//Ÿ���� ��ġ����
	POINT ptIndex;
	ptIndex.x = LONG(vMousePos.x / TILECX);
	ptIndex.y = LONG(vMousePos.y / TILECY);

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
	////������ �������� ���⺤�͸� ������.
	//D3DXVECTOR3 vDirection[4] = 
	//{
	//	vPoint[1] - vPoint[0],
	//	vPoint[2] - vPoint[1],
	//	vPoint[3] - vPoint[2],
	//	vPoint[0] - vPoint[3]
	//};
	//
	//
	////�������͸� ����� ����.
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
	////�������ͷ� ����� ����.
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
	//	//������ ����� ����� Ÿ���� �ܺ��̰�.
	//	//				������ Ÿ���� �����̴�.
	//	
	//	if(fDotResult > 0.f)
	//		return false;
	//}

	return true;
}

HRESULT CBackground::Initialize(void)
{
	//int iA = 20;
	//if(10 = iA)
	//{
	//}

	this->LoadTileData();

	m_pBackgroundTexture = CTextureMgr::GetInstance()->GetTexture( L"Map" );

	//for(int i = 0; i < TILEY; ++i)	
	//{
	//	for(int j = 0; j < TILEX; ++j)
	//	{
	//		TILE* pTile = new TILE;
	//
	//		//float fX = j * TILECX + ((i % 2) * (TILECX / 2.f));
	//		//float fY = i * (TILECY * 0.5f);
	//
	//		float fX = float(j * TILECX);
	//		float fY = float(i * TILECY);
	//
	//		pTile->vPos = D3DXVECTOR3(fX, fY, 0.f);
	//		pTile->vSize = D3DXVECTOR3( (float)TILECX, (float)TILECY, 0.f );
	//
	//		pTile->byOption = 0;
	//		pTile->byDrawID = 0;
	//
	//		m_vecTile.push_back(pTile);
	//	}
	//}

	return S_OK;
}

int CBackground::Update(void)
{
	return 0;
}

void CBackground::Render(void)
{
	D3DXMATRIX matTrans;

	TCHAR szIndexText[MIDDLE_STR] = L"";

	D3DXMatrixTranslation( &matTrans, 0.f, 0.f, 0.f );
	CDevice::GetInstance()->GetSprite()->SetTransform( &matTrans );

	RECT rc = { (LONG)m_vScroll.x, (LONG)m_vScroll.y, WINCX, WINCY };
	rc.right += rc.left;
	rc.bottom += rc.top;

	CDevice::GetInstance()->GetSprite()->Draw( m_pBackgroundTexture->pTexture, &rc, NULL, NULL, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

	int iStartY = (LONG)((m_vScroll.y) / TILECY);
	int iEndY = (LONG)((m_vScroll.y + WINCY) / TILECY);

	int iStartX = (LONG)((m_vScroll.x) / (TILECX));
	int iEndX = (LONG)((m_vScroll.x + WINCX) / (TILECX));
	//return;

	for(int i = iStartY; i < iEndY + 1; ++i)
	{
		for(int j = iStartX; j < iEndX + 1; ++j)
		{
			int iIndex = i * TILEX + j;

			if ( i < 0 || i >= TILEY || j < 0 || j >= TILEX )
				continue;

			const TEX_INFO* pTileTex = 
				CTextureMgr::GetInstance()->GetTexture(L"Back",
														L"Tile",
														m_vecTile[iIndex]->byDrawID);

			if ( m_vecTile[iIndex]->byDrawID == 0 )
				continue;

			D3DXMatrixTranslation(&matTrans
								   , m_vecTile[iIndex]->vPos.x - m_vScroll.x	//0 : x
								   , m_vecTile[iIndex]->vPos.y - m_vScroll.y	//1 : y
								   , 0.f);

			CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

			CDevice::GetInstance()->GetSprite()->Draw(
				pTileTex->pTexture,
				NULL, 
				NULL,
				NULL,
				D3DCOLOR_ARGB(255, 255, 255, 255)
			);

			////##��Ʈ ���
			//swprintf_s(szIndexText, L"%d", iIndex);
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

void CBackground::Release(void)
{
	for(size_t i = 0; i < m_vecTile.size(); ++i)
	{
		safe_delete(m_vecTile[i]);
	}
	m_vecTile.clear();
}

void CBackground::MiniMapRender(void)
{
	D3DXMATRIX matTrans, matScale, matWorld;

	const TEX_INFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture( L"BackGround" );

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
	D3DXMatrixTranslation( &matTrans, m_vScroll.x * fBackScaleX, m_vScroll.y * fBackScaleY, 0.f );

	matWorld = matScale * matTrans;

	CDevice::GetInstance()->GetSprite()->SetTransform( &matWorld );

	CDevice::GetInstance()->GetSprite()->Draw( pMiniMapAreaTex->pTexture, NULL, NULL, NULL, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

}

void CBackground::LoadTileData()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	DWORD dwByte = 0;

	HANDLE hFile = CreateFile(
		L"../Data/Tile2.dat",		
		GENERIC_READ,	
		NULL,		
		NULL,
		OPEN_EXISTING,	
		FILE_ATTRIBUTE_NORMAL,		//�⺻������ ������ �����϶�¿ɼ�.
		NULL
	);

	for(size_t i = 0; i < m_vecTile.size(); ++i)
	{
		safe_delete(m_vecTile[i]);
	}
	m_vecTile.clear();

	while(true)
	{
		TILE* pTileData = new TILE;

		ReadFile(hFile, pTileData, sizeof(TILE), &dwByte, NULL);

		if(dwByte == 0)
		{
			safe_delete(pTileData);
			break;
		}

		m_vecTile.push_back(pTileData);
	}

	CloseHandle(hFile);
}
