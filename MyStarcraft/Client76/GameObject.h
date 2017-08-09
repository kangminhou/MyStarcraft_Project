#pragma once

#include "Include.h"
#include "Transform.h"

class CGameObject
{
private:
	vector<CComponent*>	m_vecComponent;
	forward_list<CComponent*> m_listActiveComponent;

	CTransform*	m_pTransform;

	wstring		m_wstrObjKey;

	eObjectType	m_eType;

	D3DXMATRIX	m_matWorld;

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

protected:
	void SetObjKey( const wstring& _wstrObjKey );

public:
	D3DXVECTOR3 GetPos() const;
	static D3DXVECTOR3 GetScroll();
	template <typename T>
	const T* GetComponent( void ) const;
	const CTransform*	GetTransform() const;
	wstring GetObjKey( void ) const;
	eObjectType GetObjectType() const;
	D3DXMATRIX GetWorldMatrix() const;

protected:
	static D3DXVECTOR3 m_vScroll;

public:
	virtual HRESULT	Initialize(void);
	virtual int		Update(void)PURE;
	virtual void	Render(void)PURE;
	virtual void	Release(void)PURE;

public:
	void AddComponent( CComponent* _pComponent );
	void Translate( const float& _fSpeed );
	void UpdateMatrix( void );

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
