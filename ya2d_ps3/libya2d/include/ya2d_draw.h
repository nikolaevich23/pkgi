#ifndef _YA2D_DRAW_H_
#define _YA2D_DRAW_H_

#include <tiny3d.h>
#include <malloc.h>

#include "ya2d_main.h"
#include "ya2d_texture.h"
#include "ya2d_globals.h"

void ya2d_drawRectZ(int x, int y, int z, int w, int h, u32 color);
void ya2d_drawFillRectZ(int x, int y, int z, int w, int h, u32 color);

#define ya2d_drawRect(x, y, w, h, color)		ya2d_drawRectZ(x, y, YA2D_DEFAULT_Z, w, h, color)
#define ya2d_drawFillRect(x, y, w, h, color)	ya2d_drawFillRectZ(x, y, YA2D_DEFAULT_Z, w, h, color)


#endif
