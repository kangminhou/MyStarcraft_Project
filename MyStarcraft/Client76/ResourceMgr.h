#pragma once
#include "Include.h"

class CResourceObj;
class CGameEntity;

class CResourceMgr
{
	DECLARE_SINGLETON(CResourceMgr)

public:
	enum eResourceType
	{
		Resource_Mineral,
		Resource_Gas,
		Resource_End
	};

private:
	queue<CResourceObj*>	m_queueResourceObject;
	vector<CGameEntity*>	m_vecResource[Resource_End];

public:
	CResourceMgr();
	~CResourceMgr();

public:
	vector<CGameEntity*>* GetVecResource( const eResourceType& _eMakeType );

public:
	void Initialize();
	void Release();
	
public:
	CResourceObj* PopObject( const eResourceType& _eType, CGameEntity* _pEntity );
	void PushObject( CResourceObj* _pObj );
	CGameEntity* MakeResource( const D3DXVECTOR3& _vPos, const eResourceType& _eMakeType, const eObjectType& _eType );

private:
	void MakeObject();
	CResourceObj* Pop();

};

