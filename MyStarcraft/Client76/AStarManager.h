#pragma once
#include "Include.h"

class CAStar;
/* AStar 를 관리해줄 매니저.. */
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

