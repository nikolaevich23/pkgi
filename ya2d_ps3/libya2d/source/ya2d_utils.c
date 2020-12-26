#include "ya2d_utils.h"


	void swizzle_fast(u8* out, const u8* in, unsigned int width, unsigned int height) //from PSPSDK sample
	{
		unsigned int blockx, blocky;
		unsigned int j;

		unsigned int width_blocks = (width / 16);
		unsigned int height_blocks = (height / 8);

		unsigned int src_pitch = (width-16)/4;
		unsigned int src_row = width * 8;

		const u8* ysrc = in;
		u32* dst = (u32*)out;

		for (blocky = 0; blocky < height_blocks; ++blocky)
		{
			const u8* xsrc = ysrc;
			for (blockx = 0; blockx < width_blocks; ++blockx)
			{
				const u32* src = (u32*)xsrc;
				for (j = 0; j < 8; ++j)
				{
					*(dst++) = *(src++);
					*(dst++) = *(src++);
					*(dst++) = *(src++);
					*(dst++) = *(src++);
					src += src_pitch;
				}
				xsrc += 16;
			}
			ysrc += src_row;
		}
	}

	unsigned int next_pow2(unsigned int x) //from http://locklessinc.com/articles/next_pow2/
	{
		x -= 1;
		x |= (x >> 1);
		x |= (x >> 2);
		x |= (x >> 4);
		x |= (x >> 8);
		x |= (x >> 16);
		return x + 1;
	}

