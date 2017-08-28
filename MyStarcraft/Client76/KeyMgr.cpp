#include "StdAfx.h"
#include "KeyMgr.h"

#include "TimeMgr.h"

IMPLEMENT_SINGLETON(CKeyMgr)

CKeyMgr::CKeyMgr( void )
	: m_bDoubleClick( false )
	, m_bClick( false )
	, m_fIntervalClickTime( 0.5f )
{
	for ( BYTE i = 0; i < MAX_KEY_NUM; ++i )
	{
		m_bStayDownKey[i] = m_bOnceDownKey[i] = m_bUpKey[i] = false;
	}
}

CKeyMgr::~CKeyMgr(void)
{
}

bool CKeyMgr::GetKeyStayDown( const int & _vKey ) const
{
	return this->m_bStayDownKey[_vKey];
}

bool CKeyMgr::GetKeyOnceDown( const int & _vKey ) const
{
	return this->m_bOnceDownKey[_vKey];
}

bool CKeyMgr::GetKeyUp( const int & _vKey ) const
{
	return this->m_bUpKey[_vKey];
}

bool CKeyMgr::GetDoubleClick() const
{
	return this->m_bDoubleClick;
}

void CKeyMgr::Initialize()
{
	//this->m_vecCheckKey.push_back( VK_UP );
	//this->m_vecCheckKey.push_back( VK_DOWN );
	//this->m_vecCheckKey.push_back( VK_RIGHT );
	//this->m_vecCheckKey.push_back( VK_LEFT );
	//
	//this->m_vecCheckKey.push_back( VK_LBUTTON );
	//this->m_vecCheckKey.push_back( VK_RBUTTON );

	for ( int i = 0; i < 255; ++i )
		this->m_vecCheckKey.push_back( i );

	//this->m_vecCheckKey.push_back( 'S' );
}

void CKeyMgr::Update()
{
	/* 키를 땐 후에 그 다음 업데이트 시 false 로 바꿔주기 위한 코드.. */
	while ( !m_queueUpKeyDownIndex.empty() )
	{
		int iUpKeyDownIndex = m_queueUpKeyDownIndex.back();
	
		this->m_bUpKey[iUpKeyDownIndex] = false;
	
		m_queueUpKeyDownIndex.pop();
	
	}

	/* 키 체크 */
	size_t totalCheckKeyNum = this->m_vecCheckKey.size();
	//for ( BYTE i = 0; i < MAX_KEY_NUM; ++i )
	for ( size_t i = 0; i < totalCheckKeyNum; ++i )
	{
		this->KeyCheck( i );
	}

	if ( !this->m_bDoubleClick )
	{
		if ( !this->m_bClick )
		{
			if ( this->GetKeyOnceDown( VK_LBUTTON ) )
			{
				this->m_bClick = true;
				this->m_fClickTime = CTimeMgr::GetInstance()->GetGlobalTime();
			}
		}
		else
		{
			float fCurTime = CTimeMgr::GetInstance()->GetGlobalTime();
			bool bClick = false;
			if ( this->GetKeyOnceDown( VK_LBUTTON ) )
			{
				bClick = true;
			}

			if ( fCurTime - this->m_fIntervalClickTime <= m_fClickTime )
			{
				if ( bClick )
				{
					this->m_bDoubleClick = true;
					this->m_bClick = false;
				}
			}
			else
				this->m_bClick = false;
		}
	}
	else
		this->m_bDoubleClick = false;

}

void CKeyMgr::KeyCheck( int vKey )
{
	if(GetAsyncKeyState(vKey))
	{
		if ( this->m_bStayDownKey[vKey] ) // 전에 이미 누른 키라면..
			this->m_bOnceDownKey[vKey] = false;
		else{ // 전에 안 누른 키라면..
			this->m_bStayDownKey[vKey] = true;
			this->m_bOnceDownKey[vKey] = true;
		}
	}
	else // 키 안 눌렀을 때..
	{
		if(this->m_bStayDownKey[vKey]) // 현재 키가 눌렸다가 땐 상태라면..
		{
			this->m_bUpKey[vKey] = true;
			this->m_bStayDownKey[vKey] = false;
			this->m_bOnceDownKey[vKey] = false;

			this->m_queueUpKeyDownIndex.push( vKey );
		}
	}
}
