#ifndef _YA2D_MAIN_H_
#define _YA2D_MAIN_H_

#include <stdio.h>
#include <stdarg.h>
#include <tiny3d.h>
#include <libfont.h>
#include <io/pad.h>
#include <sysutil/sysutil.h>
#include <sysmodule/sysmodule.h>
#include <lv2/systime.h>
#include <sys/systime.h>

#include "valloc.h"
#include "ya2d_globals.h"
#include "ya2d_screen.h"

int ya2d_init();
int ya2d_deinit();

//inline float ya2d_getFPS();
void ya2d_error(char *t);


#endif
