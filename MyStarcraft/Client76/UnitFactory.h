#pragma once

class CGameObject;

template<typename T>
class CUnitFactory abstract
{
public:
	static CGameObject* CreateUnit( void )
	{
		CUnit* pObject = new CUnit;
		CUnitBridge* pUnitBridge = new T;
		pObject->SetUnitBridge( pUnitBridge );

		pObject->Initialize();

		return pObject;
	}

};

