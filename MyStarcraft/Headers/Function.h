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

template<typename T>
static void safe_release(T& rData)		//��۸� �����͸� �����ϱ� ���ؼ� 
{
	if(rData)
	{
		rData->Release();
		rData = NULL;
	}
}
