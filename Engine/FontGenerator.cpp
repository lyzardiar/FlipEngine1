#include "FontGenerator.h"
#include "Texture.h"

FontGenerator::FontGenerator()
{
}

FontGenerator::~FontGenerator()
{
}

void FontGenerator::init()
{
	_hWnd = GetActiveWindow();
	_defaultFont = CreateFont(20, // nHeight 
        0, // nWidth 
        0, // nEscapement 
        0, // nOrientation 
        FW_THIN, // nWeight 
        FALSE, // bItalic «„–±
        FALSE, // bUnderline 
        0, // cStrikeOut 
        ANSI_CHARSET, // nCharSet 
        OUT_DEFAULT_PRECIS, // nOutPrecision 
        CLIP_DEFAULT_PRECIS, // nClipPrecision 
        DEFAULT_QUALITY, // nQuality 
        DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
        ("Arial")); // lpszFac


	
}


Texture* FontGenerator::genFontTexture(const char* text)
{
	HDC hdc = GetDC(_hWnd);
	_hdc = CreateCompatibleDC(hdc);
	RECT rc;
	rc.top = 0;
	rc.left = 0;

	// calculate the text texture size
	int len = strlen(text);
    SelectObject(_hdc, _defaultFont);
	DrawText(_hdc, text, len, &rc, DT_LEFT|DT_NOCLIP|DT_CALCRECT);
    DeleteObject(_defaultFont);

	// create a bitmap
    if (_hBitmap)
        DeleteObject(_hBitmap);
    _hBitmap = CreateBitmap(rc.right, rc.bottom, 1, 32, NULL);

	// draw the text on the _hdc
    SelectObject(_hdc, _defaultFont);
	HGDIOBJ hOldBmp  = SelectObject(_hdc, _hBitmap);
	DrawText(_hdc, text, -1, &rc, DT_LEFT|DT_NOCLIP);
    DeleteObject(_defaultFont);
	SelectObject(_hdc, hOldBmp);

	// get pixel
    unsigned char* pData = new unsigned char[rc.right * rc.bottom * 4];

    struct
    {
		BITMAPINFOHEADER bmiHeader;
		int mask[4];
    } bi = {0};
    bi.bmiHeader.biSize = sizeof(bi.bmiHeader);

	// Get the BITMAPINFO structure from the bitmap
	if(0 == GetDIBits(_hdc, _hBitmap, 0, 0, NULL, (LPBITMAPINFO)&bi, DIB_RGB_COLORS))
	{
		// error handling
	}

	bi.bmiHeader.biHeight = abs(bi.bmiHeader.biHeight);
    GetDIBits(_hdc, _hBitmap, 0, bi.bmiHeader.biHeight, pData, (LPBITMAPINFO)&bi, DIB_RGB_COLORS);

        // change pixel's alpha value to 255, when it's RGB != 0
        COLORREF * pPixel = NULL;
        for (int y = 0; y < rc.bottom; ++y)
        {
            pPixel = (COLORREF *)pData + y * rc.right;
            for (int x = 0; x < rc.right; ++x)
            {
                COLORREF& clr = *pPixel;

                clr |= (0xffffff | (GetRValue(clr) << 24));
				clr = ~clr;
                ++pPixel;
            }
        }

	// get the texture pixels, width, height
	Texture* texture = new Texture;
	texture->init(rc.right, rc.bottom, pData);

	return texture;
}
