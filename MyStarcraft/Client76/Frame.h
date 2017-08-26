#pragma once

#include "Include.h"

// ���ʿ� ������ ������ŭ�� Ʈ�縦 �����Ѵ�.

class CFrame
{
private:
	explicit CFrame(void);
	virtual ~CFrame(void);
public:
	HRESULT Initialize(const float& fCallCnt);
	bool Permit_Call(const float& fTimeDelta);
private:
	float		m_fTimeAcc;
	float		m_fTimePerSec;

public: // ������ ���õ� �Լ�.
	static CFrame* Create(const float& fCallCnt);
public:
	void Release(void);

};
