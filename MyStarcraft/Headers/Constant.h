#pragma once

extern HWND g_hWnd;

const int WINCX = 800;
const int WINCY = 600;

const WORD MIN_STR	  = 64;
const WORD MIDDLE_STR = 128;
const WORD MAX_STR	  = 256;

const int TILECX = 32;
const int TILECY = 32;

//const int TILEX = 128;
//const int TILEY = 96;

const int TILEX = WINCX / TILECX;
const int TILEY = WINCY / TILECY;

const DWORD ITEM_WEAPON = 0x00000001;
const DWORD ITEM_ARMOR	= 0x00000002;
const DWORD ITEM_POTION = 0x00000004;

const float	Unit_Stop_Time = 1.f;
const BYTE MAX_KEY_NUM = 255;
