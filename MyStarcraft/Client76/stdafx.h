// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C�� ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.

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
#include <iostream>		//PathRelativePathTo()�� ����ϱ� ���ؼ��� ����ʿ�.
#include <io.h>
////##[17.07.18_01]

#include <fstream>

#import <msxml4.dll>

// fmod Ȱ������ ����� ���̺귯�� �߰�.
#include "fmod.h"
#include "fmod.hpp"
#pragma comment(lib, "fmodex_vc.lib")

// ������ ����� �ϱ� ���� ����� ���̺귯�� ����.
#include <Vfw.h>
#pragma comment(lib, "vfw32.lib")

#include <process.h>

//#ifdef _DEBUG
//#include <vld.h>
//#endif