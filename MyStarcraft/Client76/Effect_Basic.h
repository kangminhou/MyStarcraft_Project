#pragma once
#include "EffectBridge.h"

/*
 * Name : CEffect_Basic
 * Brief : ���� �⺻���� ����Ʈ ( ���� ��ġ�� �����ؼ� �ִϸ��̼��� ������ ������� )..
 */
class CEffect_Basic :
	public CEffectBridge
{
public:
	CEffect_Basic();
	virtual ~CEffect_Basic();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Render() override;
	virtual void Release() override;

};

