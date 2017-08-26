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

//##[17.08.02_04] : ����Ʈ���� ����
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

//A* Node ����ü�� ��������.
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

	/* ���� Ÿ��.. */

}COMMON_DATA;

typedef struct tagEntityManaData
{
	int iCurMP;
	int iMaxMP;
} ENTITY_MANA_DATA;

typedef struct tagResearchData
{
	float fResearchTime;
	int iRequireMineral;		// ���� �� �ʿ��� �̳׶�..
	int iRequireGas;			// ���� �� �ʿ��� ����..

	SHORT	nIconFrame;			// ������ ������..

	wstring	wstrName;

	vector<int>	vecRequireBuilding;		// ���� �ϱ����� �������� �ϴ� �ǹ�..

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
	// /* �ʿ�� �ϴ� �ǹ��� ����.. */
	// vector<�ǹ�> vecRequireBuilding;

	float fGenerateTime;		// ���� �ð�..
	int iRequireMineral;		// ���� �� �ʿ��� �̳׶�..
	int iRequireGas;			// ���� �� �ʿ��� ����..
	int iRequirePopulation;		// ���� �� �ʿ��� �α� ��..

	vector<int>	vecRequireBuilding;		// ���� �ϱ����� �������� �ϴ� �ǹ�..

	SHORT	nIconFrame;			// ������ ������..

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

typedef struct tagButtonData
{
	int		iFunc;			// ������ ��� ( SetPattern �Լ��� UseSkill �Լ��� ȣ���� �� ���ڷ� �ѱ� ���� )..
	int		iMessage;

	SHORT	nIconFrame;		// ������ �������� ���ΰ�..

	BYTE	byShortCutKey;	// ����Ű..
	BYTE	byDrawColumn;	// �׸� ��..

	int		iBtnKind;		// ��ư�� ������ �� ������ �ൿ ����..
	bool	bCanUse;		// ���� ��� �����Ѱ�??..
	bool	bImmediate;		// �����ڸ��� ����ϴ� ��ư??..

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

/* ���� ������.. */
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

	/* ���÷��� ������ ����.. */
	/*
	 * ���÷��� ���� ���� : https://namu.wiki/w/%EC%8A%A4%ED%83%80%ED%81%AC%EB%9E%98%ED%94%84%ED%8A%B8%20%EC%8B%9C%EB%A6%AC%EC%A6%88/%EC%8B%9C%EC%8A%A4%ED%85%9C
	 * �޴� ������ : ���� 100%, �߰� 50%, �ٱ��� 25%
	 * ������ �ȼ� ���� ( 32�ȼ� == Ÿ��1�� )
	 */
	eWeaponAttType		eWeaponAttType;
	int iInside;		// ���� ����..
	int iMiddle;		// �߰� ����..
	int iOutSide;		// �ٱ��� ����..

	eEffectShowKind		eEffectShowKind;

	BULLET_DATA*		pBulletData;

	SHORT				nIconFrame;

	bool				bImagePathInfluenceEntityDir;	// Entity ��ü�� ���⿡ ���� �ؽ��� ��ȣ�� ������ ���°�??..
	bool				bImagePath2InfluenceEntityDir;
	

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



