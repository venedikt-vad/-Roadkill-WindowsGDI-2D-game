#include "Input.h"

int keys[255];
vector<int>used_keys;

TCHAR Input_d_S[50];

POINT cCoords = { 0,0 };
POINT localcCoords = { 0,0 };

void getKeys()
{
	unsigned int i;
	for (i = 0; i < used_keys.size(); i++)
	{
		keys[used_keys[i]] = GetAsyncKeyState(used_keys[i]);
	}
}

void MousePos(POINT position) {
	SetCursorPos(position.x, position.y);
}

void holdMouse(POINT position) {
	GetCursorPos(&cCoords);
	cCoords.x -= position.x;
	cCoords.y -= position.y;
	SetCursorPos(position.x, position.y);
}

void scanKeys()
{
	for (int i = 0; i < 255; i++) {
		if (GetAsyncKeyState(i) != 0) 
			swprintf_s(Input_d_S, TEXT("key = %d"), i);
	}
}

void getMousePos()
{
	GetCursorPos(&localcCoords);
	//ScreenToClient(hWnd, &localcCoords);
}
