#pragma once
#include "GameObject.h"
class CBuilding :
	public CGameObject
{
private:
	/* 
	 * �ǹ��� �Ǽ� ���� �ִϸ��̼��� ������ ����..
	 * 
	 */

public:
	CBuilding();
	virtual ~CBuilding();

	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT	Initialize(void) override;
	virtual int Update( void ) override;
	virtual void Render( void ) override;
	virtual void Release( void ) override;

};

