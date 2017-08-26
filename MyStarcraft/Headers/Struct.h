#pragma once

typedef struct tagInfo
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vDir;
	D3DXVECTOR3 vLook;
	D3DXVECTOR3 vSize;
	
	tagInfo(void){}

	tagInfo(D3DXVECTOR3 _vPos, D3DXVECTOR3 _vSize)
		:vPos(_vPos)
		,vSize(_vSize)
	{}


}INFO, *PINFO, *pINFO;

typedef struct tagMouseDragData
{
	D3DXVECTOR3 vStartPos;
	D3DXVECTOR3 vEndPos;
}MOUSE_DRAG_DATA;

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

//##[17.08.02_04] : 리스트원소 삭제
struct DeleteObj 
{
	template<typename T>
	void operator() (T& rData)
	{
		if(rData)
		{
			safe_delete(rData);
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

//A* Node 구조체를 선언하자.
typedef struct tagAStarNode 
{
	float			fCost;
	float			fGCost;
	int				iIndex;
	BYTE			byEntityTileData;
	tagAStarNode*	pParent;

}NODE;


typedef struct tagCommonData
{
	float fCurHp;
	float fMaxHp;

	float fSpeed;

	int iDefense;
	int iUpgradeDefense;
	int iScope;

	SHORT	nDefenceIconFrame;

	eArmorUpgradeType eArmorUpgradeType;

	tagCommonData()
		: eArmorUpgradeType(Upgrade_Armor_End)
		, iUpgradeDefense(0)
	{}

	/* 유닛 타입.. */

}COMMON_DATA;

typedef struct tagEntityManaData
{
	int iCurMP;
	int iMaxMP;
} ENTITY_MANA_DATA;

typedef struct tagResearchData
{
	float fResearchTime;
	int iRequireMineral;		// 생성 시 필요한 미네랄..
	int iRequireGas;			// 생성 시 필요한 가스..

	SHORT	nIconFrame;			// 아이콘 프레임..

	wstring	wstrName;

	vector<int>	vecRequireBuilding;		// 생성 하기위해 지어져야 하는 건물..

	tagResearchData()
		: wstrName(L"")
	{}

	tagResearchData(const float& _fResearchTime, const int& _iRequireMineral, const int& _iRequireGas, const wstring& _wstrName)
		: fResearchTime(_fResearchTime)
		, iRequireMineral(_iRequireMineral)
		, iRequireGas(_iRequireGas)
		, wstrName(_wstrName)
	{}

}RESEARCH_DATA;

typedef struct tagUnitGenerateData
{
	// /* 필요로 하는 건물들 변수.. */
	// vector<건물> vecRequireBuilding;

	float fGenerateTime;		// 생성 시간..
	int iRequireMineral;		// 생성 시 필요한 미네랄..
	int iRequireGas;			// 생성 시 필요한 가스..
	int iRequirePopulation;		// 생성 시 필요한 인구 수..

	vector<int>	vecRequireBuilding;		// 생성 하기위해 지어져야 하는 건물..

	SHORT	nIconFrame;			// 아이콘 프레임..

	tagUnitGenerateData()
	{}

	tagUnitGenerateData(const float& _fGenerateTime, const int& _iRequireMineral, const int& _iRequireGas,
						 const int& _iRequirePopulation, const SHORT& _nIconFrame)
		: fGenerateTime(_fGenerateTime * 0.1f)
		, iRequireMineral(_iRequireMineral)
		, iRequireGas(_iRequireGas)
		, iRequirePopulation(_iRequirePopulation)
		, nIconFrame(_nIconFrame)
	{}

}UNIT_GENERATE_DATA;

typedef struct tagUnitSelectShowData
{
	wstring wstrName;
	wstring wstrRank;

	tagUnitSelectShowData()
		: wstrName(L"")
		, wstrRank(L"")
	{}

	tagUnitSelectShowData(const wstring& _wstrName, const wstring& _wstrRank)
		: wstrName(_wstrName)
		, wstrRank(_wstrRank)
	{}

}SELECT_UNIT_SHOW_DATA;

typedef struct tagHpBarUIMatrixData
{
	D3DXMATRIX						hpBarBackWordMatrix;
	vector<pair<BYTE, D3DXMATRIX>>	vecHpBarData;
}HP_BAR_MATRIX_DATA;


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

typedef struct tagButtonData
{
	int		iFunc;			// 실행할 기능 ( SetPattern 함수나 UseSkill 함수를 호출할 때 인자로 넘길 예정 )..
	int		iMessage;

	SHORT	nIconFrame;		// 아이콘 프레임이 몇인가..

	BYTE	byShortCutKey;	// 단축키..
	BYTE	byDrawColumn;	// 그릴 행..

	int		iBtnKind;		// 버튼을 눌렀을 때 실행할 행동 종류..
	bool	bCanUse;		// 현재 사용 가능한가??..
	bool	bImmediate;		// 누르자마자 사용하는 버튼??..

	tagButtonData()
	{}

	tagButtonData( const int& _iFunc, const SHORT& _nIconFrame, const BYTE& _byShortCutKey, const BYTE& _byDrawColumn,
				   const int& _iBtnKind, const bool& _bCanUse, const int& _iMessage = -1, const bool& _bImmediate = false )
		: iFunc( _iFunc )
		, nIconFrame( _nIconFrame )
		, byShortCutKey( _byShortCutKey )
		, byDrawColumn( _byDrawColumn )
		, iBtnKind( _iBtnKind )
		, bCanUse( _bCanUse )
		, iMessage(_iMessage)
		, bImmediate(_bImmediate)
	{}

}BUTTON_DATA;

typedef struct tagBulletData
{
	float				fCastingTime;
	float				fSpeed;

	IMAGE_PATH*			pCastingImagePath;
	IMAGE_PATH*			pEffectImaePath;
	IMAGE_PATH*			pImagePath;

	bool				bUseFollowDustEffect;

	tagBulletData()
		: bUseFollowDustEffect(false)
		, pImagePath(nullptr)
		, pCastingImagePath(nullptr)
		, pEffectImaePath(nullptr)
		, fSpeed(0.f)
		, fCastingTime(0.f)
	{}

} BULLET_DATA;

/* 무기 데이터.. */
typedef struct tagWeaponData
{
	float fDamage;
	float fAttInterval;

	float fUpgradeDamage = 0.f;

	eWeaponUpgradeType eUpgradeType;
	int iUpgradePlus;
	int iUpgradeNum = 0;

	int iMinAttRange;
	int iMaxAttRange;

	wstring						wstrWeaponName;
	IMAGE_PATH*					pImagePath;
	IMAGE_PATH*					pImagePath2;

	/* 스플래시 데미지 여부.. */
	/*
	 * 스플래시 정보 참고 : https://namu.wiki/w/%EC%8A%A4%ED%83%80%ED%81%AC%EB%9E%98%ED%94%84%ED%8A%B8%20%EC%8B%9C%EB%A6%AC%EC%A6%88/%EC%8B%9C%EC%8A%A4%ED%85%9C
	 * 받는 데미지 : 안쪽 100%, 중간 50%, 바깥쪽 25%
	 * 범위는 픽셀 단위 ( 32픽셀 == 타일1개 )
	 */
	eWeaponAttType		eWeaponAttType;
	int iInside;		// 안쪽 범위..
	int iMiddle;		// 중간 범위..
	int iOutSide;		// 바깥쪽 범위..

	eEffectShowKind		eEffectShowKind;

	BULLET_DATA*		pBulletData;

	SHORT				nIconFrame;

	bool				bImagePathInfluenceEntityDir;	// Entity 객체의 방향에 따라 텍스쳐 번호에 영향이 가는가??..
	bool				bImagePath2InfluenceEntityDir;
	

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



