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

INFO* pInfo;		//둘다 동일하다.
PINFO pInfo;
pINFO pInfo;
*/

//Texture를 보관하는 구조체를 만들어 보자.
typedef struct tagTexture
{
	LPDIRECT3DTEXTURE9	pTexture;			//이미지 한장을 보관하기 위한 변수.
	D3DXIMAGE_INFO		ImageInfo;		//이미지의 정보를 보관하기 위한 변수.
}TEX_INFO;

typedef struct tagFrame
{
	float fIndex;		//현재 스프라이트의 인덱스
	float fCount;		//1초동안 반복되는 프레임의 수.(재생속도)
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


//타일 정보를 보관할 구조체.
typedef struct tagTile
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vSize;

	BYTE		byOption;		// 충돌 여부(갈수있다, 없다, 포탈:마을,상점.., 이벤트)..
	BYTE		byDrawID;		// 타일 이미지..
	BYTE		byHeight;		// 타일의 높이..

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

	/* 유닛 타입.. */

}COMMON_DATA;

typedef struct tagUnitGenerateData
{
	// /* 필요로 하는 건물들 변수.. */
	// vector<건물> vecRequireBuilding;

	float fGenerateTime;		// 생성 시간..
	int iRequireMineral;		// 생성 시 필요한 미네랄..
	int iRequireGas;			// 생성 시 필요한 가스..
	int iRequirePopulation;		// 생성 시 필요한 인구 수..
}UNIT_GENERATE_DATA;


//[17.07.21_06] : 파일이름을 찾기 위한 구조체
typedef struct tagImagePath
{
	wstring		wstrObjKey;		//이미지의 종류(ex.Player, Monster...)
	wstring		wstrStateKey;	//이미지의 상태(Att, Idle, Run...)
	wstring		wstrPath;		//이미지의 경로
	int			iCount;			//이미지 장수

	tagImagePath( void )
		:wstrObjKey( L"" )
		, wstrStateKey( L"" )
		, wstrPath( L"" )
		, iCount( 0 )
	{}

}IMAGE_PATH;

/* 무기 데이터.. */
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

//상대경로를 추출하는 함수.
inline wstring RelativePath( wstring szFullPath )
{
	TCHAR szRelativePath[MAX_PATH] = L"";
	TCHAR szDirectoryPath[MAX_PATH] = L"";


	//프로젝트가 있는 위치까지의 경로를 얻어올수 있는 함수.
	GetCurrentDirectory( sizeof( szDirectoryPath ), szDirectoryPath );


	//상대경로를 추출해주는 함수.
	PathRelativePathTo( szRelativePath,
						szDirectoryPath,
						FILE_ATTRIBUTE_DIRECTORY,
						szFullPath.c_str(),
						FILE_ATTRIBUTE_NORMAL );

	return szRelativePath;
}


//[17.07.21_07]
//폴더안에 있는 파일(이미지파일_png)의 개수를 계산해주는 함수.
static int DirectoryFileCount( const wstring& wstrPath )
{
	//Png파일만 찾고 싶다.
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
		//다음파일이 있는지를 검사.

		//디렉토리 유무룰 검사하자.
		if ( !Find.IsDirectory() )
			++iFileCount;
	}

	return iFileCount;
}


//폴더의 정보를 추출하는 기능의 함수.
static void DirectoryInfoExtraction( const wstring& wstrPath,
									 list<IMAGE_PATH*>& rPathInfoList )
{
	//MFC에서 지원해주는 클래스.
	CFileFind	Find;

	Find.FindFile( wstrPath.c_str() );

	int iContinue = 1;

	while ( iContinue )
	{
		iContinue = Find.FindNextFile();
		//FindNextFile() : 파일 유무를 검사하는 함수.

		if ( Find.IsDots() )		//내 경로중에 .과 ..을 걸러내기 위한 함수.
			continue;

		//검색되는 파일이 폴더인지? 파일인지를 구분하자.
		if ( Find.IsDirectory() )		//현재 파일이 폴더이다.
		{
			//재귀함수를 통해서 파일을 찾을때까지 호출하도록 하자.

			DirectoryInfoExtraction( wstring( Find.GetFilePath() + L"\\*.*" ), rPathInfoList );
		}
		else		//파일
		{
			//파일을 찾았지만 찾은 파일이 시스템 파일인 경우에 예외처리를 하자.
			if ( Find.IsSystem() )
				continue;

			IMAGE_PATH*		pImagePath = new IMAGE_PATH;

			TCHAR	szPathBuffer[MAX_PATH] = L"";


			//추출한 경로를 문자열로 복사를 하자.
			lstrcpy( szPathBuffer, Find.GetFilePath().operator LPCWSTR() );
			//\Texture\Stage\Effect\BossMultiAttack\BossMultiAttack0.png


			//제일 위에 있는 경로를 잘라내는 함수.
			PathRemoveFileSpec( szPathBuffer );
			//\Texture\Stage\Effect\BossMultiAttack

			//PathRemoveFileSpec(szPathBuffer);
			////\Texture\Stage\Effect

			pImagePath->iCount = DirectoryFileCount( szPathBuffer );


			//현재 Find
			//\Texture\Stage\Effect\BossMultiAttack\BossMultiAttack0.png

			//파일이름을 얻어오자.
			wstring wstrImageName = Find.GetFileTitle().operator LPCWSTR();
			//wstrIamgeName -> BossMultiAttack0


			//BossMultiAttack000 -> BossMultiAttack%d
			wstrImageName = wstrImageName.substr( 0, wstrImageName.size() - 1 ) + L"%d.png";
			//substr() : 문자열을 잘라주는 함수.
			
			PathCombine( szPathBuffer, szPathBuffer, wstrImageName.c_str() );
			//두 문자열을 결합해주는 함수.


			//상대경로로 변경을 해보도록 하자.
			pImagePath->wstrPath = RelativePath( szPathBuffer );
			//\Texture\Stage\Effect\BossMultiAttack\BossMultiAttack%d.png


			PathRemoveFileSpec( szPathBuffer );
			//\Texture\Stage\Effect\BossMultiAttack


			pImagePath->wstrStateKey = PathFindFileName( szPathBuffer );
			//\Texture\Stage\Effect\ ->BossMultiAttack


			PathRemoveFileSpec( szPathBuffer );
			//\Texture\Stage\Effect

			pImagePath->wstrObjKey = PathFindFileName( szPathBuffer );

			/* szPath 와 szPathBuffer 의 값이 똑같은데 중복으로 씀(선생님의도(?) or 실수(?)) */
			////경로를 얻어오자.(%d로 바꾼 경로를 얻어오자)
			//TCHAR szPath[MAX_PATH] = L"";
			//
			//lstrcpy( szPath, Find.GetFilePath().operator LPCWSTR() );
			////\Texture\Stage\Effect\BossMultiAttack\BossMultiAttack0.png
			//
			//PathRemoveFileSpec( szPath );
			////\Texture\Stage\Effect\BossMultiAttack\
			//			//현재는 절대경로를 받아오고있다.
			//
			//PathCombine( szPath, szPath, wstrImageName.c_str() );
			////두 문자열을 결합해주는 함수.
			//
			//
			////상대경로로 변경을 해보도록 하자.
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


			//현재 파일에서 하나 위단계를 찾아가자.
			Find.FindFile( Find.GetFilePath() );


			//다음폴더로 넘어가자
			Find.FindNextFile();
		}
	}
}
#endif



