#pragma once
#include <windows.h>
#include <vector>

#include <iostream>

using namespace std;

extern TCHAR Input_d_S[50];

#define key_w 87
#define key_s 83
#define key_a 65
#define key_d 68

#define key_up VK_UP
#define key_down VK_DOWN
#define key_left VK_LEFT
#define key_right VK_RIGHT

#define key_semicolon 188
#define key_point 190
#define key_m 77

#define key_esc 27

#define mouse_L VK_LBUTTON
#define mouse_R VK_RBUTTON

#define key_shift_L VK_LSHIFT
#define key_space VK_SPACE
#define key_alt 164
#define key_enter 13
#define key_tab 9

extern int keys[255];
extern vector<int>used_keys;

extern POINT cCoords;
extern POINT localcCoords;

void getKeys();

void MousePos(POINT position);

void holdMouse(POINT position);

void getMousePos();

void scanKeys();