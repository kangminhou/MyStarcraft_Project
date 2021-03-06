// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C의 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

#include <d3dx9math.h>

#include <list>
#include <vector>
#include <map>
#include <stack>
#include <queue>
#include <forward_list>

#include <algorithm>
#include <string>
#include <iostream>		//PathRelativePathTo()를 사용하기 위해서는 헤더필요.
#include <io.h>
////##[17.07.18_01]

#include <fstream>

#import <msxml4.dll>

// fmod 활용위한 헤더와 라이브러리 추가.
#include "fmod.h"
#include "fmod.hpp"
#pragma comment(lib, "fmodex_vc.lib")

// 동영상 재생을 하기 위한 헤더와 라이브러리 포함.
#include <Vfw.h>
#pragma comment(lib, "vfw32.lib")

#include <process.h>

//#ifdef _DEBUG
//#include <vld.h>
//#endif