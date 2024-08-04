#pragma once
#include <vector>
#include <Windows.h>
#pragma comment(lib, "Msimg32.lib")

#define TRANSPARENT_COLOR RGB(1, 2, 3)

class IMG {
public:
	HDC hdcImg;

	HBITMAP Img;
	BITMAP Imgbm;

	IMG(LPCWSTR path, HDC hdc);

	void loadImg(LPCWSTR path, HDC hdc);
	void drawImg(HDC hdc, COORD position);
	void drawImgT(HDC hdc, COORD position);
};