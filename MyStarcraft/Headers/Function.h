#pragma once

//template<class T>
template<typename T>
static void safe_delete(T& rData)		//댕글리 포인터를 방지하기 위해서 
{
	if(rData)
	{
		delete rData;
		rData = NULL;
	}
}
