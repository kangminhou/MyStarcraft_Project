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
	LPD3DXSPRITE			m_pTempSprite;		// �ӵ��� ���� ������ ������ (�ǵ��� �ʱ�)...
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
	 * GetComponent : ## ���ƾƾƾƾƾƾƾ�
						 ������Ʈ�� �θ� Ŭ������ ã�� �� ��������(���ƾƾƾƾӾƾƾƾƾƾ�)..
						 typeid ���� �ٸ� ������� �ذ��ؾ���....
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
	* AddComponent : ������Ʈ �߰����ִ� �Լ�..

	* �Ű�����
	 - _pComponent : �߰��� ������Ʈ..
	*/
	void AddComponent( CComponent* _pComponent );

	/*
	 * Translate : ������Ʈ�� ���� �ٶ󺸰� �ִ� �������� �̵�..

	 * �Ű�����
	  - _fSpeed : �̵��� ���ǵ� ( �� )..
	 */
	void Translate(const float& _fSpeed);
	void Translate(const D3DXVECTOR3& _vMove);

	/*
	* UpdateMatrix : ���� ������Ʈ�� Transform ������ �������� ���� ��� ����..
	*/
	void UpdateMatrix( void );

protected:
	/*
	 * DrawTexture : Texture �� Device �� ���� ���ִ� �Լ� ( ������ �̹��� ���� �ڵ�� �����ϱ� ������ ( ����.. ) )..

	 * �Ű�����
	  - _pDrawTexture : ��� Texture..
	  - _matWorld : Texture �� ��� ��ǥ..
	  - _vCenterPos : �׸��� �߰� ��ġ ( ������ �� 0, 0, 0 ���� )..
	  - _color : �׸� �׸� �� ( ���� ���ϸ� �׳� �� �̹��� �״�� )..
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
