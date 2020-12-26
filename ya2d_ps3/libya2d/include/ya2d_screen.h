#ifndef _YA2D_SCREEN_H_
#define _YA2D_SCREEN_H_

#include <lv2/systime.h>
#include <tiny3d.h>
#include "ya2d_globals.h"


void ya2d_screenClear();
void ya2d_screenSetClearColor(u32 new_color);
void ya2d_screenFlip();
void ya2d_screenBeginDrawing();
float ya2d_screenFPS();


#endif
