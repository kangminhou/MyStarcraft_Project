#pragma once
#include "Component.h"
#include "Include.h"

class CTransform :
	public CComponent
{
public:
	enum eMoveKind { Move_World, Move_Local };

private:
	INFO		m_tInfo;

public:
	CTransform();
	virtual ~CTransform();

public:
	const INFO*	GetInfo();
	D3DXVECTOR3 GetPos() const;
	D3DXVECTOR3 GetDir() const;
	D3DXVECTOR3 GetLook() const;
	D3DXVECTOR3 GetSize() const;

public:
	void SetPos( const D3DXVECTOR3& _vPos );
	void SetPos( const float& _fX, const float& _fY );
	void SetLook( const D3DXVECTOR3& _vLook );
	void SetLook( const float& _fX, const float& _fY );
	void SetDir( const D3DXVECTOR3& _vDir );
	void SetDir( const float& _fX, const float& _fY );
	void SetSize( const D3DXVECTOR3& _vSize );
	void SetSize( const float& _fX, const float& _fY );

public:
	// CComponent을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Release() override;

public:
	void Translate( float _fSpeed, eMoveKind _eMoveKind );
	void UpdateTransform();

};

