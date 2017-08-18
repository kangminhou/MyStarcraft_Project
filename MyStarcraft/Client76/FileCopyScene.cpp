#include "stdafx.h"
#include "FileCopyScene.h"


CFileCopyScene::CFileCopyScene()
{
}


CFileCopyScene::~CFileCopyScene()
{
}

HRESULT CFileCopyScene::Initialize( void )
{
	int iCnt = 0;

	for ( int i = 0; i < 17; ++i )
	{
		for ( int j = 0; j < 7; ++j )
		{
			sprintf_s( m_szImageFileName, "../Texture/Effect/SiegeTankFire/Test/STankFire%d.png", j * 17 + i );
			sprintf_s( m_szCopyFileName, "../Texture/Effect/SiegeTankFire/STankFire%d.png", iCnt++ );

			if ( this->FileCopy() )
			{
				cout << "복사성공!!!" << endl;
			}
			else
			{
				cout << "복사실패!!!" << endl;
				return E_FAIL;
			}
		}
	}

	return S_OK;
}

int CFileCopyScene::Update( void )
{

	return 0;
}

void CFileCopyScene::Render( void )
{
	
}

void CFileCopyScene::Release( void )
{
	
}

errno_t CFileCopyScene::FileOpen( FILE** ppFp, char szFileName[], char szMode[] )
{
	if ( NULL == ppFp )
		return -1;

	errno_t err = fopen_s( ppFp, szFileName, szMode );

	return err;

}

bool CFileCopyScene::FileCopy()
{
	if ( 0 != FileOpen( &m_pOpenFile, m_szImageFileName, "rb" ) )
		return 0;

	if ( 0 != FileOpen( &m_pCopyFile, m_szCopyFileName, "wb" ) )
	{
		fclose( m_pOpenFile );
		return 0;

	}

	while ( true )
	{
		int iTemp = fgetc( m_pOpenFile );

		if ( EOF == iTemp )
			break;

		fputc( iTemp, m_pCopyFile );

	}

	fclose( m_pOpenFile );
	fclose( m_pCopyFile );

	return true;
}
