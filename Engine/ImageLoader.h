#pragma once
class Image;

bool loadImagePNG(const char *file, Image& i);

bool loadImageJPG(const char *file, Image& i);

bool loadImageBMP(const char *file, Image& i);

bool loadImageDDS(const char *file, Image& i);