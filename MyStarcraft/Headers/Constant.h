#pragma once

extern HWND g_hWnd;

const BOOL BWindowed = TRUE;
const BOOL FOG = TRUE;

const int WINCX = 800;
const int WINCY = 600;

const WORD MIN_STR	  = 64;
const WORD MIDDLE_STR = 128;
const WORD MAX_STR	  = 256;

const int TILECX = 32;
const int TILECY = 32;

const int TILEX = 128;
const int TILEY = 96;

const int SPACECX = 100;
const int SPACECY = 100;

const int SPACEX = (TILEX * TILECX + (TILECX - 1)) / SPACECX;
const int SPACEY = (TILEY * TILECY + (TILECY - 1)) / SPACECY;

const int TOTAL_SPACE_NUM = SPACEX * SPACEY;

//const int TILEX = WINCX / TILECX;
//const int TILEY = WINCY / TILECY;

const DWORD ITEM_WEAPON = 0x00000001;
const DWORD ITEM_ARMOR	= 0x00000002;
const DWORD ITEM_POTION = 0x00000004;

const float	Unit_Stop_Time = 1.f;
const BYTE MAX_KEY_NUM = 255;

const float EPSILON = 0.00001f;

const float HpUI_Level2 = 0.6f;
const float HpUI_Level3 = 0.3f;

const int Minimap_CX = 160;
const int Minimap_CY = 160;
