#include "stdafx.h"
#include "Background.h"

#include "Device.h"
#include "TextureMgr.h"

#include "GameEntity.h"


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

void CBackground::TileChange(const D3DXVECTOR3& vMousePos
							  , const int& iOption, const int& iHeight)
{
	int iTileIndex = GetTileIndex(vMousePos);

	if(iTileIndex == -1)
		return;

	m_vecTile[iTileIndex]->byOption = iOption;		//갈수 없다.
	m_vecTile[iTileIndex]->byHeight = iHeight;

	if ( iOption == 0 )
		m_vecTile[iTileIndex]->byDrawID = 0;
	else
		m_vecTile[iTileIndex]->byDrawID = 1;
}

bool CBackground::Picking(const D3DXVECTOR3& vMousePos,
						   const int& iIndex)
{
	//타일의 위치벡터
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

//void CBackground::EraseUnitData( const CGameEntity * _pEntity, const D3DXVECTOR3 & _vPos )
//{
//	D3DXVECTOR3 vEntityPos = _vPos;
//	RECT rcEntityOriginCol = _pEntity->GetOriginColRect();
//
//	int iStartX = int(vEntityPos.x + rcEntityOriginCol.left) / (TILECX / ENTITY_TILE_DIV_X);
//	int iEndX = int(vEntityPos.x + rcEntityOriginCol.right) / (TILECX / ENTITY_TILE_DIV_X);
//
//	int iStartY = int(vEntityPos.y + rcEntityOriginCol.top) / (TILECY / ENTITY_TILE_DIV_Y);
//	int iEndY = int(vEntityPos.y + rcEntityOriginCol.bottom) / (TILECY / ENTITY_TILE_DIV_Y);
//
//	for ( int i = iStartY; i <= iEndY; ++i )
//	{
//		for ( int j = iStartX; j <= iEndX; ++j )
//		{
//			if ( i < 0 || i >= TILEY ||
//				 j < 0 || j >= TILEX )
//				continue;
//
//			int iIndex = j + i * (TILEX * ENTITY_TILE_DIV_X);
//
//			auto& checkList = m_entityTileData[iIndex].entityList;
//			auto iter_end = checkList.end();
//			for ( auto iter = checkList.begin(); iter != iter_end; ++iter )
//			{
//				if ( (*iter) == _pEntity )
//				{
//					checkList.erase( iter );
//					if ( checkList.empty() )
//						m_entityTileData[iIndex].byTileOption = 0;
//
//					return 0;
//				}
//			}
//		}
//	}
//}

void CBackground::EraseUnitData( const vector<_Dlong>& _vecKey )
{
	if ( _vecKey.empty() )
		return;

	size_t iLength = _vecKey.size();
	for ( size_t i = 0; i < iLength; ++i )
	{
		_Dlong iIndex = _vecKey[i] >> 32;
		
		if ( iIndex < 0 )
			continue;
			
		_Dlong iEntityArrIndex = ((_vecKey[i] << 32) >> 32);
		m_entityTileData[iIndex].pEntityArr[iEntityArrIndex] = NULL;

		bool bEmptySpace = true;
		/* 현재 타일의 있는 유닛이 없는지 체크.. */
		for ( int i = 0; i < ENTITY_TOTAL_TILE_DIV; ++i )
		{
			if ( m_entityTileData[iIndex].pEntityArr[i] )
			{
				bEmptySpace = false;
				break;
			}
		}

		if ( bEmptySpace )
			m_entityTileData[iIndex].byTileOption = 0;
	}


}

void CBackground::UpdateUnitData( const CGameEntity * _pEntity, vector<_Dlong>& _vecKey )
{
	_vecKey.clear();

	if ( _pEntity->GetIsDie() )
		return;

	RECT rcEntityCurCol = _pEntity->GetColRect();

	int iStartX = (rcEntityCurCol.left) / (TILECX / ENTITY_TILE_DIV_X);
	int iEndX = (rcEntityCurCol.right) / (TILECX / ENTITY_TILE_DIV_X);

	int iStartY = (rcEntityCurCol.top) / (TILECY / ENTITY_TILE_DIV_Y);
	int iEndY = (rcEntityCurCol.bottom) / (TILECY / ENTITY_TILE_DIV_Y);

	list<pair<int, BYTE>> indexList;

	for ( int i = iStartY; i <= iEndY; ++i )
	{
		for ( int j = iStartX; j <= iEndX; ++j )
		{
			if ( i < 0 || i >= TILEY * ENTITY_TILE_DIV_Y ||
				 j < 0 || j >= TILEX * ENTITY_TILE_DIV_X )
				continue;

			_Dlong iSpaceKey = -1;
			_Dlong iIndex = j + i * (TILEX * ENTITY_TILE_DIV_X);
			bool bFindSameEntity = false;
			BYTE byDetailSpaceData = 0;

			int xDetailIndex = j % ENTITY_TILE_DIV_X;
			int yDetailIndex = i % ENTITY_TILE_DIV_Y;

			if ( xDetailIndex == 0 && yDetailIndex == 0 )
				byDetailSpaceData = (1 << 0);
			else if ( xDetailIndex == 1 && yDetailIndex == 0 )
				byDetailSpaceData = (1 << 1);
			else if ( xDetailIndex == 0 && yDetailIndex == 1 )
				byDetailSpaceData = (1 << 2);
			else if ( xDetailIndex == 1 && yDetailIndex == 1 )
				byDetailSpaceData = (1 << 3);

			int iInputIndex = (j / ENTITY_TILE_DIV_Y) + (i / ENTITY_TILE_DIV_X) * TILEX;

			bool bFind = false;
			for each (auto iter in indexList)
			{
				if ( iter.first == iInputIndex )
				{
					bFind = true;
					iter.second |= byDetailSpaceData;
					break;
				}
			}

			if ( !bFind )
				indexList.push_back( pair<int, BYTE>( iInputIndex, byDetailSpaceData ) );

			for ( int i = 0; i < ENTITY_TOTAL_TILE_DIV; ++i )
			{
				if ( m_entityTileData[iIndex].pEntityArr[i] == NULL )
				{
					if ( m_entityTileData[iIndex].byTileOption == 0 )
						m_entityTileData[iIndex].byTileOption = 1;

					m_entityTileData[iIndex].pEntityArr[i] = _pEntity;

					iSpaceKey = iIndex << 32;
					iSpaceKey += i;
					_vecKey.push_back( iSpaceKey );
				}
			}
		}
	}

	const_cast<CGameEntity*>(_pEntity)->SetStandTileIndexList( indexList );
}

bool CBackground::CheckCanGoTile( const int & _iIndex, const BYTE & _byDir, CGameEntity * _pEntity, const bool& _bCheckEntityData )
{
	/* 인덱스가 타일 범위 밖의 것이라면 ( 예외처리.. ) */
	if ( _iIndex < 0 || (unsigned int)_iIndex >= this->m_vecTile.size() )
		return false;

	if ( this->m_vecTile[_iIndex]->byOption != 0 )
	{
		if ( !_bCheckEntityData && this->m_vecTile[_iIndex]->byOption == 4 )
			return true;

		return false;
	}

	if ( !_bCheckEntityData )
		return true;

	BYTE byEntityTileData = this->CalcEntityTileData( _iIndex, _pEntity );

	if ( byEntityTileData == 15 )
		return false;

	switch ( _byDir )
	{
		case 0:
		return true;

		case 1:	// 위쪽..
		{
			BYTE byStartTileEntityData = this->CalcEntityTileData( _iIndex + TILECX, _pEntity );

			if ( byStartTileEntityData & (1 << 0) &&
				 byStartTileEntityData & (1 << 1) )
				return false;

			if ( (byEntityTileData & (1 << 2) &&
				 byEntityTileData & (1 << 3)) )
				return false;
		}
		break;

		case 2:	// 아래쪽..
		{
			BYTE byStartTileEntityData = this->CalcEntityTileData( _iIndex - TILECX, _pEntity );

			if ( byStartTileEntityData & (1 << 2) &&
				 byStartTileEntityData & (1 << 3) )
				return false;

			if ( (byEntityTileData & (1 << 0) &&
				 byEntityTileData & (1 << 1)) )
				return false;
		}
		break;

		case 3:	// 왼쪽..
		{
			BYTE byStartTileEntityData = this->CalcEntityTileData( _iIndex + 1, _pEntity );

			if ( byStartTileEntityData & (1 << 1) &&
				 byStartTileEntityData & (1 << 3) )
				return false;

			if ( (byEntityTileData & (1 << 1) &&
				 byEntityTileData & (1 << 3)) )
				return false;
		}
		break;

		case 4:	// 오른쪽..
		{
			BYTE byStartTileEntityData = this->CalcEntityTileData( _iIndex - 1, _pEntity );

			if ( byStartTileEntityData & (1 << 0) &&
				 byStartTileEntityData & (1 << 2) )
				return false;

			if ( (byEntityTileData & (1 << 2) &&
				 byEntityTileData & (1 << 4)) )
				return false;
		}
		break;

		case 5: case 6: case 7: case 8:// 오른쪽 위..
		{
			if ( byEntityTileData != 0 )
				return false;
		}
		break;

		//case 6:	// 오른쪽 아래..
		//{
		//	if ( this->m_entityTileData[xIndex + ((yIndex + 1) * (TILEX * ENTITY_TILE_DIV_X))].byTileOption == 1 &&
		//		 this->m_entityTileData[xIndex + 1 + ((yIndex + 1) * (TILEX * ENTITY_TILE_DIV_X))].byTileOption == 1 )
		//		return false;
		//}
		//	break;
		//case 7:	// 왼쪽 아래..
		//{
		//	if ( this->m_entityTileData[xIndex + ((yIndex + 1) * (TILEX * ENTITY_TILE_DIV_X))].byTileOption == 1 &&
		//		 this->m_entityTileData[xIndex + 1 + ((yIndex + 1) * (TILEX * ENTITY_TILE_DIV_X))].byTileOption == 1 )
		//		return false;
		//}
		//	break;
		//case 8:	// 왼쪽 위..
		//{
		//	if ( this->m_entityTileData[xIndex + ((yIndex + 1) * (TILEX * ENTITY_TILE_DIV_X))].byTileOption == 1 &&
		//		 this->m_entityTileData[xIndex + 1 + ((yIndex + 1) * (TILEX * ENTITY_TILE_DIV_X))].byTileOption == 1 )
		//		return false;
		//}
		//	break;
	}

	return true;
}

BYTE CBackground::CalcEntityTileData( const int & _iIndex, CGameEntity * _pEntity )
{
	BYTE byOut = 0;

	int yIndex = (_iIndex / TILEX);
	int xIndex = (_iIndex - yIndex * TILEX);

	yIndex *= ENTITY_TILE_DIV_Y;
	xIndex *= ENTITY_TILE_DIV_X;

	ENTITY_TILE_DATA& entityCheckTileLeftUp = this->m_entityTileData[xIndex + ((yIndex) * (TILEX * ENTITY_TILE_DIV_X))];
	ENTITY_TILE_DATA& entityCheckTileRightUp = this->m_entityTileData[xIndex + 1 + ((yIndex) * (TILEX * ENTITY_TILE_DIV_X))];
	ENTITY_TILE_DATA& entityCheckTileLeftDown = this->m_entityTileData[xIndex + ((yIndex  + 1) * (TILEX * ENTITY_TILE_DIV_X))];
	ENTITY_TILE_DATA& entityCheckTileRightDown = this->m_entityTileData[xIndex + 1 + ((yIndex + 1) * (TILEX * ENTITY_TILE_DIV_X))];

	/* 못가는 타일 검사.. */
	if ( entityCheckTileLeftUp.byTileOption == 1 )
		byOut |= (1 << 0);
	if ( entityCheckTileRightUp.byTileOption == 1 )
		byOut |= (1 << 1);
	if ( entityCheckTileLeftDown.byTileOption == 1 )
		byOut |= (1 << 2);
	if ( entityCheckTileRightDown.byTileOption == 1 )
		byOut |= (1 << 3);

	if ( _pEntity )
	{
		/* 현재 Entity 가 밟고 있는 타일인지 아닌지 검사.. */
		const list<pair<int, BYTE>>* pCheckTileIndexList = _pEntity->GetStandTileIndexList();
		
		for each (auto iter in (*pCheckTileIndexList))
		{
			if ( iter.first == _iIndex )
			{
				byOut -= iter.second;
				break;
			}
		}

	}

	return byOut;
}

int CBackground::CalcNearCanGoTile( const int & _iStartIndex, const int & _iEndIndex, const bool& _bCheckEntityData )
{
	TILE* pStartTile = this->m_vecTile[_iStartIndex];
	TILE* pEndTile = this->m_vecTile[_iEndIndex];

	D3DXVECTOR3 vDir;
	D3DXVec3Normalize( &vDir, &(pEndTile->vPos - pStartTile->vPos) );

	float fStartYIndex = (float)(_iStartIndex / TILEX);
	float fStartXIndex = (float)(_iStartIndex - fStartYIndex * TILEX);

	float fEndYIndex = (float)(_iEndIndex / TILEX);
	float fEndXIndex = (float)(_iEndIndex - fEndYIndex * TILEX);

	D3DXVec3Normalize( &vDir, &(D3DXVECTOR3( fStartXIndex, fStartYIndex, 0.f ) - D3DXVECTOR3( fEndXIndex, fEndYIndex, 0.f )) );

	float fXIndex = fEndXIndex;
	float fYIndex = fEndYIndex;

	int iIndex = 0;

	while ( true )
	{
		fXIndex += vDir.x;
		fYIndex += vDir.y;

		iIndex = (int)((int)fXIndex + ((int)(fYIndex)) * TILEX);

		if ( this->CheckCanGoTile( iIndex, 0, NULL, _bCheckEntityData ) )
			return iIndex;

		if ( abs( fStartXIndex - fXIndex ) <= 1.f && abs( fStartYIndex - fYIndex ) <= 1.f )
			return -1;

	}

	return -1;
}

bool CBackground::CheckConnectPath( CGameEntity* _pEntity, const int & _iStartIndex, const int & _iEndIndex, const bool & _bCheckEntityData )
{
	TILE* pStartTile = this->m_vecTile[_iStartIndex];
	TILE* pEndTile = this->m_vecTile[_iEndIndex];

	if ( !this->CheckCanGoTile( _iEndIndex, 8, NULL, _bCheckEntityData ) )
		return false;

	D3DXVECTOR3 vDir;
	D3DXVec3Normalize( &vDir, &(pEndTile->vPos - pStartTile->vPos) );

	float fStartYIndex = (float)(_iStartIndex / TILEX);
	float fStartXIndex = (float)(_iStartIndex - fStartYIndex * TILEX);

	float fEndYIndex = (float)(_iEndIndex / TILEX);
	float fEndXIndex = (float)(_iEndIndex - fEndYIndex * TILEX);

	D3DXVec3Normalize( &vDir, &(D3DXVECTOR3( fStartXIndex, fStartYIndex, 0.f ) - D3DXVECTOR3( fEndXIndex, fEndYIndex, 0.f )) );

	float fXIndex = fEndXIndex;
	float fYIndex = fEndYIndex;

	int iIndex = 0;

	TILE* pCurTile = nullptr;

	while ( true )
	{
		fXIndex += vDir.x;
		fYIndex += vDir.y;

		iIndex = (int)((int)fXIndex + ((int)(fYIndex)) * TILEX);

		pCurTile = this->m_vecTile[iIndex];

		if ( !this->CheckCanGoTile( iIndex, 8, _pEntity, _bCheckEntityData ) )
			return false;

		if ( abs( fStartXIndex - fXIndex ) <= 1.f && abs( fStartYIndex - fYIndex ) <= 1.f )
			break;

	}

	return true;
}

void CBackground::ObjectDataUpdate( CGameEntity * pEntity, int iTileOption /*= 1*/ )
{
	RECT rcCol = pEntity->GetColRect();
	D3DXVECTOR3 vPos = pEntity->GetPos();

	int iStartX = int(vPos.x + rcCol.left)   / TILECX;
	int iEndX =   int(vPos.x + rcCol.right - 1)  / TILECX;
	int iStartY = int(vPos.y + rcCol.top)    / TILECY;
	int iEndY =   int(vPos.y + rcCol.bottom) / TILECY;

	for ( int i = iStartY; i < iEndY; ++i )
	{
		for ( int j = iStartX; j <= iEndX; ++j )
		{
			int iIndex = j + i * TILEX;
			this->m_vecTile[iIndex]->byOption = iTileOption;
			this->m_vecTile[iIndex]->byDrawID = 1;
		}
	}
}

HRESULT CBackground::Initialize(void)
{
	//int iA = 20;
	//if(10 = iA)
	//{
	//}

	this->LoadTileData();

	m_pBackgroundTexture = CTextureMgr::GetInstance()->GetTexture( L"BackGround" );

	for ( int i = 0; i < 16; ++i )
		m_vecTileTexture.push_back( CTextureMgr::GetInstance()->GetTexture( L"Back", L"Tile", i ) );

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
	return;

	for(int i = iStartY; i < iEndY + 1; ++i)
	{
		for(int j = iStartX; j < iEndX + 1; ++j)
		{
			int iIndex = i * TILEX + j;

			if ( i < 0 || i >= TILEY || j < 0 || j >= TILEX )
				continue;

			BYTE byDrawID = this->m_vecTile[iIndex]->byDrawID;
			BYTE byEntityTIleData = this->CalcEntityTileData( iIndex, NULL );

			if ( this->m_vecTile[iIndex]->byDrawID == 0 )
			{
				switch ( byEntityTIleData )
				{
					case 1:
						byDrawID = 15;
						break;
					case 2:
						byDrawID = 14;
						break;
					case 3:
						byDrawID = 8;
						break;
					case 4:
						byDrawID = 13;
						break;
					case 5:
						byDrawID = 9;
						break;
					case 6:
						byDrawID = 10;
						break;
					case 7:
						byDrawID = 5;
						break;
					case 8:
						byDrawID = 12;
						break;
					case 9:
						byDrawID = 11;
						break;
					case 10:
						byDrawID = 7;
						break;
					case 11:
						byDrawID = 4;
						break;
					case 12:
						byDrawID = 6;
						break;
					case 13:
						byDrawID = 3;
						break;
					case 14:
						byDrawID = 2;
						break;
					case 15:
						byDrawID = 1;
						break;
				}
			}

			const TEX_INFO* pTileTex = this->m_vecTileTexture[byDrawID];

			//if ( m_vecTile[iIndex]->byDrawID == 0 )
			//	continue;

			D3DXMatrixTranslation(&matTrans
								   , m_vecTile[iIndex]->vPos.x - m_vScroll.x	//0 : x
								   , m_vecTile[iIndex]->vPos.y - m_vScroll.y	//1 : y
								   , 0.f);

			CDevice::GetInstance()->GetSprite()->SetTransform( &matTrans );

			CDevice::GetInstance()->GetSprite()->Draw(
				pTileTex->pTexture,
				NULL,
				NULL,
				NULL,
				D3DCOLOR_ARGB( 255, 255, 255, 255 )
			);

			////##폰트 출력
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	DWORD dwByte = 0;

	HANDLE hFile = CreateFile(
		L"../Data/Tile.dat",		
		GENERIC_READ,	
		NULL,		
		NULL,
		OPEN_EXISTING,	
		FILE_ATTRIBUTE_NORMAL,		//기본값으로 파일을 생성하라는옵션.
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
