#ifndef _YA2D_UTILS_H_
#define _YA2D_UTILS_H_

#include <tiny3d.h>

#include "ya2d_main.h"
#include "ya2d_globals.h"


void swizzle_fast(u8* out, const u8* in, unsigned int width, unsigned int height); //from PSPSDK sample
unsigned int __attribute__((noinline)) next_pow2(unsigned int x); //from http://locklessinc.com/articles/next_pow2/


#endif
