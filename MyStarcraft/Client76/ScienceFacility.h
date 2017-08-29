#pragma once
#include "Building.h"

class CScienceFacility :
	public CBuilding
{
public:
	CScienceFacility();
	virtual ~CScienceFacility();

public:
	virtual HRESULT Initialize( void ) override;
	virtual int Update( void ) override;
	virtual void Render( void ) override;
	virtual void Release( void ) override;

public:
	virtual void UpdatePosition( const D3DXVECTOR3& vPrevPos ) override;
	// CBuilding��(��) ���� ��ӵ�
	virtual void SuccessBuild() override;

public:
	// CBuilding��(��) ���� ��ӵ�
	virtual void SetPattern( const eGameEntityPattern & _ePatternKind, const bool & _bPrevPattern = FALSE ) override;

protected:
	virtual void DecideTexture();

	virtual void InitAnimation() override;
	virtual void InitPattern() override;
	virtual void InitTexture() override;

};
