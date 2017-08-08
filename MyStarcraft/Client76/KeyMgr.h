#pragma once
#include "Include.h"

class CKeyMgr
{
private:
	bool	m_bStayDownKey[MAX_KEY_NUM];
	bool	m_bOnceDownKey[MAX_KEY_NUM];
	bool	m_bUpKey[MAX_KEY_NUM];

	queue<BYTE> m_queueUpKeyDownIndex;
	vector<BYTE> m_vecCheckKey;

	DECLARE_SINGLETON(CKeyMgr)
public:
	CKeyMgr(void);
	~CKeyMgr(void);

public:
	DWORD GetKeyStayDown( int vKey ) { return m_bStayDownKey[vKey]; }
	DWORD GetKeyOnceDown( int vKey ) { return m_bOnceDownKey[vKey]; }
	DWORD GetKeyUp( int vKey ) { return m_bUpKey[vKey]; }

public:
	void Initialize();
	void Update();
	void KeyCheck( int vKey );

};
