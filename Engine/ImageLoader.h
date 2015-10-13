#ifndef __IMAGELOADER_H__
#define __IMAGELOADER_H__
class Image;

bool loadImagePNG(const char *file, Image& i);

bool loadImageJPG(const char *file, Image& i);

bool loadImageBMP(const char *file, Image& i);

bool loadImageDDS(const char *file, Image& i);

bool loadImageTGA(const char *file, Image& i);
#endif