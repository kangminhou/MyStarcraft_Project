#pragma once
#include "Include.h"

class CAStar;
/* AStar �� �������� �Ŵ���.. */
class CAStarManager
{
	DECLARE_SINGLETON(CAStarManager)

	queue<CAStar*> m_vecAStar;

public:
	CAStarManager();
	~CAStarManager();

	void AddAStar();

	void Initialize();
	void Release();

};

