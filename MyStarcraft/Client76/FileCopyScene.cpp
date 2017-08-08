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
	return S_OK;
}

int CFileCopyScene::Update( void )
{
	int iCnt = 0;
	int iCnt2 = 2;

	for ( int i = 0; i <= 50; )
	{
		FILE* fpOpen = NULL, *fpCopy = NULL;
		char szImageFileName[128] = "";
		char szCopyFileName[128] = "";
		if ( i <= 99 )
			sprintf_s( szImageFileName, "../Texture/Marine/Attack/New/marine%d.png", i );
		else
			sprintf_s( szImageFileName, "../Texture/Marine/Attack/New/marine%d.png", i );

		sprintf_s( szCopyFileName, "../Texture/Marine/Attack/Marine%d.png", iCnt++ );

		if ( 0 != FileOpen( &fpOpen, szImageFileName, "rb" ) )
			return 0;

		if ( 0 != FileOpen( &fpCopy, szCopyFileName, "wb" ) )
		{
			fclose( fpOpen );
			return 0;

		}

		while ( true )
		{
			int iTemp = fgetc( fpOpen );

			if ( EOF == iTemp )
				break;

			fputc( iTemp, fpCopy );

		}

		fclose( fpOpen );
		fclose( fpCopy );

		cout << "복사성공!!!!" << endl;

		if ( iCnt2 <= 0 )
		{
			iCnt2 = 2;
			i += 4;
		}
		else
		{
			--iCnt2;
			i++;
		}
		//if ( i < 144 )
		//	i += 2;
		//else
		//	i += 1;

	}

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