#pragma once

class CEffect;
class CEffectBridge;

template<class T>
class CEffectFactory abstract
{
public:
	static CEffect* CreateEffect(void)
	{
		CEffect* pEffect = new CEffect;
		CEffectBridge*	pEffectBridge = new T;
		pEffect->SetEffectBridge( pEffectBridge );

		return pEffect;
	}
	
};

