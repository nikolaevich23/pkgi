#ifndef _YA2D_GLOBALS_H_
#define _YA2D_GLOBALS_H_

#ifndef RGBA
	#define RGBA(r,g,b,a) ((r<<24) |(g<<16)|(b<<8)|(a))
#endif

#ifndef RGB
	#define RGB(r,g,b) RGBA(r,g,b,255)
#endif

#define ya2d_millis() (sysGetSystemTime()/1000)

#define YA2D_ERROR_BUFSIZE 256
#define YA2D_DEFAULT_FONT 0
#define YA2D_DEFAULT_Z 65535

#define YA2D_VERTEX_BUFFER_MEMORY     (1  * 1024 * 1024)
#define YA2D_RSX_TEXTURE_MEMORY       (64 * 1024 * 1024)
#define YA2D_RSX_STATIC_MEMORY_SIZE   (8  * 1024 * 1024)
#define YA2D_RSX_DYNAMIC_MEMORY_SIZE  (YA2D_RSX_TEXTURE_MEMORY-YA2D_RSX_STATIC_MEMORY_SIZE)


extern u32* ya2d_textureMem;
extern u32* ya2d_texturePointer;
extern unsigned char console_font_16x32[16384];

#endif
