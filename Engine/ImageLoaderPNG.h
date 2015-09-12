#pragma once
class Image;
enum ECOLOR_FORMAT
{
	ECF_A1R5G5B5 = 0,
	ECF_R5G6B5,
	ECF_R8G8B8,
	ECF_A8R8G8B8,
	ECF_R16F,
	ECF_G16R16F,
	ECF_A16B16G16R16F,
	ECF_R32F,
	ECF_G32R32F,
	ECF_A32B32G32R32F,
	ECF_UNKNOWN
};

class ImageLoaderPNG
{
public:
	ImageLoaderPNG();
	~ImageLoaderPNG();

	void loadImage(const char* pszFileName);

public:
	int				m_nImageWidth;
	int				m_nImageHeight;
	unsigned char*	m_pImageData;
	ECOLOR_FORMAT   m_ColorFormat;
};

bool LoadImagePNG(const char* file, Image& i);