#pragma once
#include "EffectBridge.h"

/*
 * Name : CEffect_Basic
 * Brief : 가장 기본적인 이펙트 ( 일정 위치에 등장해서 애니메이션이 끝나면 사라지는 )..
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

