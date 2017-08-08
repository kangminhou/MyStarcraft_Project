#pragma once

typedef struct tagInfo
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vDir;
	D3DXVECTOR3 vLook;
	D3DXVECTOR3 vSize;	

	D3DXMATRIX matWorld;
	
	tagInfo(void){}

	tagInfo(D3DXVECTOR3 _vPos, D3DXVECTOR3 _vSize)
		:vPos(_vPos)
		,vSize(_vSize)
	{}


}INFO, *PINFO, *pINFO;

/*
INFO tInfo;

INFO* pInfo;		//�Ѵ� �����ϴ�.
PINFO pInfo;
pINFO pInfo;
*/

//Texture�� �����ϴ� ����ü�� ����� ����.
typedef struct tagTexture
{
	LPDIRECT3DTEXTURE9	pTexture;			//�̹��� ������ �����ϱ� ���� ����.
	D3DXIMAGE_INFO		ImageInfo;		//�̹����� ������ �����ϱ� ���� ����.
}TEX_INFO;

typedef struct tagFrame
{
	float fIndex;		//���� ��������Ʈ�� �ε���
	float fCount;		//1�ʵ��� �ݺ��Ǵ� �������� ��.(����ӵ�)
	float fStart;
	float fMax;

	//tagFrame(void){}
	tagFrame( float _fIndex = 0.f, float _fCount = 0.f, float _fMax = 0.f, float _fStart = 0.f )
		:fIndex( _fIndex )
		, fCount( _fCount )
		, fMax( _fMax )
		, fStart( _fStart )
	{}

}FRAME, *PFRAME;


typedef struct tagStringCompare
{
	const TCHAR* m_pString;

	template<typename T>
	bool operator () (T rData)
	{
		return !lstrcmp(rData.first, m_pString);
	}

	tagStringCompare(const TCHAR* pKey)
		:m_pString(pKey){}
}STRING_COMPARE;


struct DeleteMap 
{
	template<typename T>
	void operator () (T& Temp)
	{
		if(Temp.second)
		{
			delete [] Temp.first;

			delete Temp.second;
			Temp.second = NULL;
		}
	}
};


//Ÿ�� ������ ������ ����ü.
typedef struct tagTile
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vSize;

	BYTE		byOption;		// �浹 ����(�����ִ�, ����, ��Ż:����,����.., �̺�Ʈ)..
	BYTE		byDrawID;		// Ÿ�� �̹���..
	BYTE		byHeight;		// Ÿ���� ����..

	tagTile()
		: byHeight(0), byDrawID(0), byOption(0)
	{}

}TILE, *PTILE, *pTILE;


typedef struct tagCommonData
{
	int iCurHp;
	int iMaxHp;

	float fSpeed;

	int iDefense;
	int iScope;

	/* ���� Ÿ��.. */

}COMMON_DATA;

typedef struct tagUnitGenerateData
{
	// /* �ʿ�� �ϴ� �ǹ��� ����.. */
	// vector<�ǹ�> vecRequireBuilding;

	float fGenerateTime;		// ���� �ð�..
	int iRequireMineral;		// ���� �� �ʿ��� �̳׶�..
	int iRequireGas;			// ���� �� �ʿ��� ����..
	int iRequirePopulation;		// ���� �� �ʿ��� �α� ��..
}UNIT_GENERATE_DATA;


//[17.07.21_06] : �����̸��� ã�� ���� ����ü
typedef struct tagImagePath
{
	wstring		wstrObjKey;		//�̹����� ����(ex.Player, Monster...)
	wstring		wstrStateKey;	//�̹����� ����(Att, Idle, Run...)
	wstring		wstrPath;		//�̹����� ���
	int			iCount;			//�̹��� ���

	tagImagePath( void )
		:wstrObjKey( L"" )
		, wstrStateKey( L"" )
		, wstrPath( L"" )
		, iCount( 0 )
	{}

}IMAGE_PATH;

/* ���� ������.. */
typedef struct tagWeaponData
{
	int iDamage;
	int iUpgradePlus;
	int iMinAttRange;
	int iMaxAttRange;

	float fAttInterval;

	eWeaponUpgradeType eUpgradeType;

} WEAPON_DATA;


#ifdef _PROJECT_MFC_

//����θ� �����ϴ� �Լ�.
inline wstring RelativePath( wstring szFullPath )
{
	TCHAR szRelativePath[MAX_PATH] = L"";
	TCHAR szDirectoryPath[MAX_PATH] = L"";


	//������Ʈ�� �ִ� ��ġ������ ��θ� ���ü� �ִ� �Լ�.
	GetCurrentDirectory( sizeof( szDirectoryPath ), szDirectoryPath );


	//����θ� �������ִ� �Լ�.
	PathRelativePathTo( szRelativePath,
						szDirectoryPath,
						FILE_ATTRIBUTE_DIRECTORY,
						szFullPath.c_str(),
						FILE_ATTRIBUTE_NORMAL );

	return szRelativePath;
}


//[17.07.21_07]
//�����ȿ� �ִ� ����(�̹�������_png)�� ������ ������ִ� �Լ�.
static int DirectoryFileCount( const wstring& wstrPath )
{
	//Png���ϸ� ã�� �ʹ�.
	//\Texture\Stage\Effect\BossMultiAttack

	//\Texture\Stage\Effect\BossMultiAttack\*.png
	wstring wstrTemp = wstrPath + L"\\*.png";

	CFileFind Find;
	Find.FindFile( wstrTemp.c_str() );

	int iContinue = 1;
	int iFileCount = 0;

	while ( iContinue )
	{
		iContinue = Find.FindNextFile();
		//���������� �ִ����� �˻�.

		//���丮 ������ �˻�����.
		if ( !Find.IsDirectory() )
			++iFileCount;
	}

	return iFileCount;
}


//������ ������ �����ϴ� ����� �Լ�.
static void DirectoryInfoExtraction( const wstring& wstrPath,
									 list<IMAGE_PATH*>& rPathInfoList )
{
	//MFC���� �������ִ� Ŭ����.
	CFileFind	Find;

	Find.FindFile( wstrPath.c_str() );

	int iContinue = 1;

	while ( iContinue )
	{
		iContinue = Find.FindNextFile();
		//FindNextFile() : ���� ������ �˻��ϴ� �Լ�.

		if ( Find.IsDots() )		//�� ����߿� .�� ..�� �ɷ����� ���� �Լ�.
			continue;

		//�˻��Ǵ� ������ ��������? ���������� ��������.
		if ( Find.IsDirectory() )		//���� ������ �����̴�.
		{
			//����Լ��� ���ؼ� ������ ã�������� ȣ���ϵ��� ����.

			DirectoryInfoExtraction( wstring( Find.GetFilePath() + L"\\*.*" ), rPathInfoList );
		}
		else		//����
		{
			//������ ã������ ã�� ������ �ý��� ������ ��쿡 ����ó���� ����.
			if ( Find.IsSystem() )
				continue;

			IMAGE_PATH*		pImagePath = new IMAGE_PATH;

			TCHAR	szPathBuffer[MAX_PATH] = L"";


			//������ ��θ� ���ڿ��� ���縦 ����.
			lstrcpy( szPathBuffer, Find.GetFilePath().operator LPCWSTR() );
			//\Texture\Stage\Effect\BossMultiAttack\BossMultiAttack0.png


			//���� ���� �ִ� ��θ� �߶󳻴� �Լ�.
			PathRemoveFileSpec( szPathBuffer );
			//\Texture\Stage\Effect\BossMultiAttack

			//PathRemoveFileSpec(szPathBuffer);
			////\Texture\Stage\Effect

			pImagePath->iCount = DirectoryFileCount( szPathBuffer );


			//���� Find
			//\Texture\Stage\Effect\BossMultiAttack\BossMultiAttack0.png

			//�����̸��� ������.
			wstring wstrImageName = Find.GetFileTitle().operator LPCWSTR();
			//wstrIamgeName -> BossMultiAttack0


			//BossMultiAttack000 -> BossMultiAttack%d
			wstrImageName = wstrImageName.substr( 0, wstrImageName.size() - 1 ) + L"%d.png";
			//substr() : ���ڿ��� �߶��ִ� �Լ�.
			
			PathCombine( szPathBuffer, szPathBuffer, wstrImageName.c_str() );
			//�� ���ڿ��� �������ִ� �Լ�.


			//����η� ������ �غ����� ����.
			pImagePath->wstrPath = RelativePath( szPathBuffer );
			//\Texture\Stage\Effect\BossMultiAttack\BossMultiAttack%d.png


			PathRemoveFileSpec( szPathBuffer );
			//\Texture\Stage\Effect\BossMultiAttack


			pImagePath->wstrStateKey = PathFindFileName( szPathBuffer );
			//\Texture\Stage\Effect\ ->BossMultiAttack


			PathRemoveFileSpec( szPathBuffer );
			//\Texture\Stage\Effect

			pImagePath->wstrObjKey = PathFindFileName( szPathBuffer );

			/* szPath �� szPathBuffer �� ���� �Ȱ����� �ߺ����� ��(�������ǵ�(?) or �Ǽ�(?)) */
			////��θ� ������.(%d�� �ٲ� ��θ� ������)
			//TCHAR szPath[MAX_PATH] = L"";
			//
			//lstrcpy( szPath, Find.GetFilePath().operator LPCWSTR() );
			////\Texture\Stage\Effect\BossMultiAttack\BossMultiAttack0.png
			//
			//PathRemoveFileSpec( szPath );
			////\Texture\Stage\Effect\BossMultiAttack\
			//			//����� �����θ� �޾ƿ����ִ�.
			//
			//PathCombine( szPath, szPath, wstrImageName.c_str() );
			////�� ���ڿ��� �������ִ� �Լ�.
			//
			//
			////����η� ������ �غ����� ����.
			//pImagePath->wstrPath = RelativePath( szPath );
			////\Texture\Stage\Effect\BossMultiAttack\BossMultiAttack%d.png
			//
			//
			//PathRemoveFileSpec( szPath );
			////\Texture\Stage\Effect\BossMultiAttack
			//
			//
			//pImagePath->wstrStateKey = PathFindFileName( szPath );
			////\Texture\Stage\Effect\ ->BossMultiAttack
			//
			//
			//PathRemoveFileSpec( szPath );
			////\Texture\Stage\Effect
			//
			//pImagePath->wstrObjKey = PathFindFileName( szPath );

			rPathInfoList.push_back( pImagePath );


			//���� ���Ͽ��� �ϳ� ���ܰ踦 ã�ư���.
			Find.FindFile( Find.GetFilePath() );


			//���������� �Ѿ��
			Find.FindNextFile();
		}
	}
}
#endif



