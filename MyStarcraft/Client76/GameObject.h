#pragma once

#include "Include.h"
#include "Transform.h"

typedef LONGLONG _Dlong;

class CGameObject
{
private:
	vector<CComponent*>	m_vecComponent;
	forward_list<CComponent*> m_listActiveComponent;

	CTransform*	m_pTransform;

	wstring		m_wstrObjKey;

	eObjectType	m_eType;

	D3DXMATRIX	m_matWorld;

	const TEX_INFO*			m_pDragTexture;

protected:
	LPD3DXSPRITE			m_pTempSprite;		// 속도를 위해 가져온 포인터 (건들지 않기)...
	LPD3DXFONT				m_pFont;

public:
	void SetPos( const D3DXVECTOR3& _vPos );
	void SetPos( const float& _fX, const float& _fY );
	void SetLook( const D3DXVECTOR3& _vLook );
	void SetLook( const float& _fX, const float& _fY );
	void SetDir( const D3DXVECTOR3& _vDir );
	void SetDir( const float& _fX, const float& _fY );
	void SetSize( const D3DXVECTOR3& _vSize );
	void SetSize( const float& _fX, const float& _fY );
	void SetObjectType( const eObjectType& eType );
	void SetMatrix( const D3DXMATRIX& _matWorld );

protected:
	void SetObjKey( const wstring& _wstrObjKey );

public:
	D3DXVECTOR3 GetPos() const;
	static D3DXVECTOR3 GetScroll();

	/*
	 * GetComponent : ## 으아아아아아아아아
						 컴포넌트의 부모 클래스로 찾을 때 문제생김(으아아아아앙아아아아아앙)..
						 typeid 말고 다른 방법으로 해결해야함....
	 */
	template <typename T>
	const T* GetComponent( void ) const;
	const CTransform*	GetTransform() const;
	wstring GetObjKey( void ) const;
	eObjectType GetObjectType() const;
	D3DXMATRIX GetWorldMatrix() const;

protected:
	static D3DXVECTOR3 m_vScroll;
	static bool	m_bScrollMove;

public:
	virtual HRESULT	Initialize(void);
	virtual int		Update(void)PURE;
	virtual void	Render(void)PURE;
	virtual void	Release(void)PURE;

public:
	virtual void UpdatePosition( const D3DXVECTOR3& vPrevPos );

public:
	/*
	* AddComponent : 컴포넌트 추가해주는 함수..

	* 매개변수
	 - _pComponent : 추가할 컴포넌트..
	*/
	void AddComponent( CComponent* _pComponent );

	/*
	 * Translate : 오브젝트가 현재 바라보고 있는 방향으로 이동..

	 * 매개변수
	  - _fSpeed : 이동할 스피드 ( 양 )..
	 */
	void Translate(const float& _fSpeed);
	void Translate(const D3DXVECTOR3& _vMove);

	/*
	* UpdateMatrix : 현재 오브젝트의 Transform 정보를 바탕으로 최종 행렬 갱신..
	*/
	void UpdateMatrix( void );

protected:
	/*
	 * DrawTexture : Texture 를 Device 에 띄우게 해주는 함수 ( 어차피 이미지 띄우는 코드는 동일하기 때문에 ( 귀찮.. ) )..

	 * 매개변수
	  - _pDrawTexture : 띄울 Texture..
	  - _matWorld : Texture 를 띄울 좌표..
	  - _vCenterPos : 그림의 중간 위치 ( 없으면 걍 0, 0, 0 으로 )..
	  - _color : 그릴 그림 색 ( 지정 안하면 그냥 그 이미지 그대로 )..
	 */
	void DrawTexture( const TEX_INFO* _pDrawTexture, 
					  const D3DXMATRIX& _matWorld,
					  const D3DXVECTOR3& _vCenterPos = D3DXVECTOR3( 0.f, 0.f, 0.f ), 
					  const D3DCOLOR& _color = D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

	void DrawRect( const RECT& _rc, const D3DCOLOR _color = D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

	void DrawFont( const D3DXMATRIX& _matWorld,
				   const wstring& wstrShow,
				   const D3DCOLOR& _color = D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

public:
	CGameObject(void);
	virtual ~CGameObject(void);

};

template<typename T>
inline const T* CGameObject::GetComponent( void ) const
{
	T* pOut = NULL;

	size_t length = m_vecComponent.size();
	for ( size_t i = 0; i < length; ++i )
	{
		if ( typeid(*(m_vecComponent[i])) == typeid(T) )
		{
			pOut = dynamic_cast<T*>(m_vecComponent[i]);
		}
	}

	return pOut;
}
