#include "GUI.h"

IMG::IMG(LPCWSTR path, HDC hdc){
	loadImg(path, hdc);
}

//class IMG
void IMG::loadImg(LPCWSTR path, HDC hdc) {
	Img = (HBITMAP)LoadImage(NULL, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hdcImg = CreateCompatibleDC(hdc);
}

void IMG::drawImg(HDC hdc, COORD position) {
	SelectObject(hdcImg, Img);
	GetObject(Img, sizeof(BITMAP), &Imgbm);
	BitBlt(hdc, position.X, position.Y, Imgbm.bmWidth, Imgbm.bmHeight, hdcImg, 0, 0, SRCCOPY);
}
void IMG::drawImgT(HDC hdc, COORD position) {
	SelectObject(hdcImg, Img);
	GetObject(Img, sizeof(BITMAP), &Imgbm);
	TransparentBlt(hdc, position.X, position.Y, Imgbm.bmWidth, Imgbm.bmHeight, hdcImg, 0, 0, Imgbm.bmWidth, Imgbm.bmHeight, TRANSPARENT_COLOR);
}

//endclass IMG