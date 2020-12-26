#ifndef _YA2D_TEXTURE_H_
#define _YA2D_TEXTURE_H_


#include <tiny3d.h>
#include <malloc.h>
#include <string.h>
#include <pngdec/pngdec.h>
#include <jpgdec/jpgdec.h>

#include "ya2d_main.h"
#include "ya2d_utils.h"
#include "ya2d_globals.h"

#define YA2D_TEXTURE_SLICE 32

typedef struct
{
	int imageWidth, imageHeight;
	int textureWidth, textureHeight;
	int centerX, centerY;
	uint32_t rowBytes, dataLength;
	void *real_ptr, *data; //data is absolute VRAM pointer
	int bitDepth, colorType, format;
	u32 textureOffset;
	uint8_t hasAlpha, isSwizzled;
}ya2d_Texture;


ya2d_Texture* ya2d_createTexture(int w, int h, int format);
void ya2d_swizzleTexture(ya2d_Texture *texp);
void ya2d_setTexture(const ya2d_Texture *texp);
void ya2d_setTextureWrap(const ya2d_Texture *texp);
void ya2d_freeTexture(ya2d_Texture *texp);

ya2d_Texture* ya2d_loadPNGfromFile(const char* filename);
ya2d_Texture* ya2d_loadPNGfromBuffer(const void *buffer, u32 buf_size);
ya2d_Texture* ya2d_loadJPGfromFile(const char* filename);
ya2d_Texture* ya2d_loadJPGfromBuffer(const void *buffer, u32 buf_size);

void ya2d_drawTextureZ(const ya2d_Texture *texp, int x, int y, int z, float scale);
void ya2d_drawBlendTextureZ(const ya2d_Texture *texp, int x, int y, int z, float scale, u32 color);
void ya2d_drawRotateTextureZ(const ya2d_Texture *texp, int x, int y, int z, float angle);

#define ya2d_drawTexture(texp, x, y)				ya2d_drawTextureZ(texp, x, y, YA2D_DEFAULT_Z, 1.0f)
#define ya2d_drawRotateTexture(texp, x, y, angle)	ya2d_drawRotateTextureZ(texp, x, y, YA2D_DEFAULT_Z, angle)
#define ya2d_drawBlendTexture(texp, x, y, color)	ya2d_drawBlendTextureZ(texp, x, y, YA2D_DEFAULT_Z, 1.0f, color)


#endif
