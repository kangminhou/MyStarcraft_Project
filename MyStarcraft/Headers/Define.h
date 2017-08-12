#pragma once

#define SAFE_DELETE(p) ( if(p){ delete p; p = NULL; } )
#define SAFE_DELETE_ARRAY(p) if(p){ delete[] p; p = NULL; }

#define CENTER_POS(x) (x / 2.f)

#define DECLARE_SINGLETON(ClassName)	\
public:									\
	static ClassName* GetInstance()		\
	{									\
	if(NULL == m_pInstance)			\
	m_pInstance = new ClassName;\
	return m_pInstance;				\
	}									\
	void DestroyInstance()				\
	{									\
	if(m_pInstance)					\
		{								\
		delete m_pInstance;			\
		m_pInstance = NULL;			\
		}								\
	}									\
private:								\
	static ClassName* m_pInstance;

#define IMPLEMENT_SINGLETON(ClassName)	\
	ClassName* ClassName::m_pInstance = NULL;


#define ERROR_MSG(Text)		\
	MessageBox(g_hWnd, Text, L"Error!", MB_OK)

#define GET_TIME CTimeMgr::GetInstance()->GetTime()
#define RANDOM_INTERGER(value) CRandom::GetInstance()->GetValue(value)
#define RANDOM_RANGE_INTERGER(minValue, maxValue) CRandom::GetInstance()->GetValue(minValue, maxValue)
#define RANDOM_FLOAT(value) CRandom::GetInstance()->GetFloatValue(value)
#define RANDOM_RANGE_FLOAT(minValue, maxValue) CRandom::GetInstance()->GetFloatValue(minValue, maxValue)

#define MY_VK_HOLD 'H'
#define MY_VK_STOP 'S'
#define MY_VK_MOVE VK_RBUTTON
#define MY_VK_ATTACK 'A'
#define MY_VK_PATROL 'P'

/* 움직일 수 있는 오브젝트의 스피드에 곱함.. */
#define Calc_Entity_Speed(speed) (speed * 50.f)
#define Object_Scope_Mul 50.f
#define Calc_Weapon_Interval(fInverval) (fInverval / 15.f)