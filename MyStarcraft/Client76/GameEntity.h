#pragma once
#include "GameObject.h"

class CAnimation;
class CGameEntity :
	public CGameObject
{
protected:
	COMMON_DATA				m_tInfoData;				// 유닛 능력치 데이터..

	wstring					m_wstrStateKey;				// 현재 상태 키 ( 이미지 출력을 위한 )..
	wstring					m_wstrPatternKey;			// 현재 실행 중인 패턴 키..

	vector<const TEX_INFO*> m_vecTexture;
	CAnimation*				m_pAnimCom;

public:
	CGameEntity();
	virtual ~CGameEntity();

public:
	virtual void SetPattern( const wstring& _wstrPatternKey ) PURE;

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize( void ) override;
	virtual int Update( void ) override;
	virtual void Render( void ) override;
	virtual void Release( void ) override;

protected:
	virtual void InitAnimation() PURE;
	virtual void InitPattern() PURE;

};

