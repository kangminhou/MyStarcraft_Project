#pragma once

class CGameObject;

template<class T>
class CFactory abstract
{
public:
	static CGameObject* CreateGameObject(void)
	{
		CGameObject* pObject = new T;
		pObject->Initialize();

		return pObject;
	}

	static CGameObject* CreateGameObject(D3DXVECTOR3 vPos)
	{
		CGameObject* pObject = new T;
		pObject->Initialize();
		pObject->SetPos(vPos);

		return pObject;
	}

	static CGameObject* CreateGameObject(float fx, float fy)
	{
		CGameObject* pObject = new T;
		pObject->Initialize();
		pObject->SetPos(fx, fy);

		return pObject;
	}

};
