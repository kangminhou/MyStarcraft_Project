#pragma once

//template<class T>
template<typename T>
static void safe_delete(T& rData)		//��۸� �����͸� �����ϱ� ���ؼ� 
{
	if(rData)
	{
		delete rData;
		rData = NULL;
	}
}
