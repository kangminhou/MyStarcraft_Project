#pragma once
#include "GameObject.h"
#include "EffectMgr.h"

class CEffectBridge;

/*
* Name : CEffect
* Brief : ���ӿ��� ����� ����Ʈ���� ����ϱ� ���� Ŭ���� 
			( Bridge ������ ����Ͽ�(????) �������� ������ CEffectBridge �ʿ��� ������ )..
*/
class CEffect :
	public CGameObject
{
private:
	wstring			m_wstrStateKey;

	CEffectBridge*	m_pEffectBridge;

	CEffectMgr::eEffectKind	m_eKind;

public:
	CEffect();
	virtual ~CEffect();

public:
	void SetEffectBridge( CEffectBridge* _pEffectBridge );
	void SetStateKey( const wstring& _wstrStateKey );
	void SetVecTexture( const wstring& _wstrObjKey, const wstring& _wstrStateKey, const int& _iStart, const int& _iCount );
	void SetVecTexture( const vector<const TEX_INFO*>* _pVecTexture );
	void SetEffectType( const CEffectMgr::eEffectKind& _eKind );

public:
	wstring	GetStateKey() const;
	const CEffectBridge* GetEffectBridge() const;

public:
	virtual HRESULT Initialize( void ) override;
	virtual int Update( void ) override;
	virtual void Render( void ) override;
	virtual void Release( void ) override;



};

