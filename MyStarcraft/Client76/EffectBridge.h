#pragma once
#include "Component.h"
#include "Include.h"

class CAnimation;
class CTransform;
class CEffect;

class CEffectBridge :
	public CComponent
{
public:
	enum eEffectEvent
	{
		Event_None,
		Event_DestroyObject,
	};

protected:
	CEffect*				m_pEffect;
	CTransform*				m_pEffectTransform;
	LPD3DXSPRITE			m_pSprite;

	vector<const TEX_INFO*>	m_vecDrawTexture;
	CAnimation*				m_pAnimation;

	bool					m_bFirstInitialize;

public:
	CEffectBridge();
	virtual ~CEffectBridge();

public:
	void SetVecTexture( const wstring& _wstrObjKey, const wstring& _wstrStateKey, const int& _iStart, const int& _iCount );
	void SetVecTexture( const vector<const TEX_INFO*>* _pVecTexture );

public:
	// CComponent을(를) 통해 상속됨
	virtual void Initialize();
	virtual int Update() PURE;
	virtual void Render();
	virtual void Release();

};

