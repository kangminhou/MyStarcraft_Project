#pragma once
#include "Include.h"

class CKeyMgr
{
private:
	bool	m_bStayDownKey[MAX_KEY_NUM];
	bool	m_bOnceDownKey[MAX_KEY_NUM];
	bool	m_bUpKey[MAX_KEY_NUM];

	bool	m_bDoubleClick;
	bool	m_bClick;
	float	m_fIntervalClickTime;
	float	m_fClickTime;

	queue<BYTE> m_queueUpKeyDownIndex;
	vector<BYTE> m_vecCheckKey;

	DECLARE_SINGLETON(CKeyMgr)
public:
	CKeyMgr(void);
	~CKeyMgr(void);

public:
	bool GetKeyStayDown( const int& _vKey ) const;
	bool GetKeyOnceDown( const int& _vKey ) const;
	bool GetKeyUp( const int& _vKey ) const;

	bool GetDoubleClick() const;

public:
	void Initialize();
	void Update();
	void KeyCheck( int vKey );

};
