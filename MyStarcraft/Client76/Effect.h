#pragma once
#include "GameObject.h"

class CEffectBridge;

/*
* Name : CEffect
* Brief : 게임에서 사용할 이펙트들을 출력하기 위한 클래스 
			( Bridge 패턴을 사용하여(????) 세부적인 내용은 CEffectBridge 쪽에서 수정함 )..
*/
class CEffect :
	public CGameObject
{
private:
	wstring			m_wstrStateKey;

	CEffectBridge*	m_pEffectBridge;

public:
	CEffect();
	virtual ~CEffect();

public:
	void SetEffectBridge( CEffectBridge* _pEffectBridge );
	void SetStateKey( const wstring& _wstrStateKey );
	void SetVecTexture( const wstring& _wstrObjKey, const wstring& _wstrStateKey, const int& _iStart, const int& _iCount );
	void SetVecTexture( const vector<const TEX_INFO*>* _pVecTexture );

public:
	wstring	GetStateKey() const;
	const CEffectBridge* GetEffectBridge() const;

public:
	virtual HRESULT Initialize( void ) override;
	virtual int Update( void ) override;
	virtual void Render( void ) override;
	virtual void Release( void ) override;



};

