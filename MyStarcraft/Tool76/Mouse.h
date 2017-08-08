#pragma once

//추상클래스 : 인터페이스화(fast.승엽)

#include "include.h"

class CMouse abstract
{
//virtual void Render()=0;
public:
	static D3DXVECTOR3 GetMousePos(void)
	{
		POINT ptMouse;

		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		return D3DXVECTOR3((float)ptMouse.x, 
						   static_cast<float>(ptMouse.y), 
						   0.f);
		
		/*D3DXVECTOR3 vMousePos;

		vMousePos.x = ptMouse.x;
		vMousePos.y = ptMouse.y;
		vMousePos.z = 0.f;

		return vMousePos;*/
	}
};
