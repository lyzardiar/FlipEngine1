#pragma once
#include <windows.h>

class Texture;
class FontGenerator
{
public:
	FontGenerator();
	~FontGenerator();

	void init();

	Texture* genFontTexture(const char* text);
private:
	HWND		_hWnd;
	HDC		_hdc;
	HFONT	_defaultFont;
	HBITMAP _hBitmap;
};

