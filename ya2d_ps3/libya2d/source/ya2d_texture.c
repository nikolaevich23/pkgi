#include "ya2d_texture.h"


	ya2d_Texture* ya2d_createTexture(int w, int h, int format)
	{
		ya2d_Texture* texp = (ya2d_Texture*)malloc(sizeof(ya2d_Texture));
		
		texp->imageWidth  = w;
		texp->imageHeight = h;
		texp->textureWidth  = w;//next_pow2(w);
		texp->textureHeight = h;//next_pow2(h);
		texp->centerX = (int)(w/2);
		texp->centerY = (int)(h/2);

		texp->format = format;
		texp->isSwizzled = 0;
		texp->hasAlpha   = 1;
		
		switch(format)
		{
		case TINY3D_TEX_FORMAT_L8:
			texp->rowBytes = texp->textureWidth;
			break;
		case TINY3D_TEX_FORMAT_A1R5G5B5:
		case TINY3D_TEX_FORMAT_A4R4G4B4:
		case TINY3D_TEX_FORMAT_R5G6B5:
			texp->rowBytes = texp->textureWidth * 2;
			break;
		case TINY3D_TEX_FORMAT_A8R8G8B8:
		default:
			texp->rowBytes = texp->textureWidth * 4;
			break;
		}
			
		texp->dataLength = texp->rowBytes * texp->textureHeight;
		texp->data = valloc(texp->dataLength);
		memset((void*)texp->data, 0x0, texp->dataLength);
		texp->textureOffset = tiny3d_TextureOffset(texp->data);
		return texp;
	}
	
	void ya2d_swizzleTexture(ya2d_Texture *texp)
	{
		if(texp->isSwizzled || (texp->textureWidth < YA2D_TEXTURE_SLICE && texp->textureHeight < YA2D_TEXTURE_SLICE)) return;
		void *swizzledData = malloc(texp->dataLength);
		swizzle_fast(swizzledData, texp->data, texp->rowBytes, texp->textureHeight);
		memcpy(texp->data, swizzledData, texp->dataLength);
		free(swizzledData);
		texp->isSwizzled = 1;
	}

    void ya2d_freeTexture(ya2d_Texture *texp)
    {
        if(texp != NULL)
        {
			if(texp->data != NULL)
			{
				vfree(texp->data);
				texp->data = NULL;
			}
			free(texp);
			texp = NULL;
        }
    }

    void ya2d_setTexture(const ya2d_Texture *texp)
    {
    	tiny3d_SetTexture(0, texp->textureOffset, texp->imageWidth, texp->imageHeight, texp->rowBytes, texp->format, TEXTURE_LINEAR);
    }

    void ya2d_setTextureWrap(const ya2d_Texture *texp)
    {
        tiny3d_SetTextureWrap(0, texp->textureOffset, texp->imageWidth, texp->imageHeight, texp->rowBytes, 
                                             texp->format, TEXTWRAP_CLAMP, TEXTWRAP_CLAMP, TEXTURE_LINEAR);
    }

    void ya2d_drawTextureZ(const ya2d_Texture *texp, int x, int y, int z, float scale)
    {
		if(!texp || !texp->data) return;

		ya2d_setTexture(texp);
		
		tiny3d_SetPolygon(TINY3D_QUADS);
			tiny3d_VertexPos(x, y, z);
			tiny3d_VertexTexture(0.0f, 0.0f);
			
			tiny3d_VertexPos(x+(texp->imageWidth * scale), y, z);
			tiny3d_VertexTexture(1.0f, 0.0f);
			
			tiny3d_VertexPos(x+(texp->imageWidth * scale), y+(texp->imageHeight * scale), z);
			tiny3d_VertexTexture(1.0f, 1.0f);
			
			tiny3d_VertexPos(x, y+(texp->imageHeight * scale), z);
			tiny3d_VertexTexture(0.0f, 1.0f);
		tiny3d_End();
    }    

	void ya2d_drawRotateTextureZ(const ya2d_Texture *texp, int x, int y, int z, float angle)
	{
		if(!texp || !texp->data) return;

		ya2d_setTexture(texp);
			
		MATRIX matrix = MatrixTranslation(x + (float)texp->centerX, y + (float)texp->centerY, 0.0f);
		matrix = MatrixMultiply(MatrixRotationZ(angle), matrix);
		
		tiny3d_SetMatrixModelView(&matrix);
		
		tiny3d_SetPolygon(TINY3D_QUADS);
			tiny3d_VertexPos((float)-texp->centerX, (float)-texp->centerY, z);
			tiny3d_VertexTexture(0.0f, 0.0f);
			
			tiny3d_VertexPos((float)texp->centerX, (float)-texp->centerY, z);
			tiny3d_VertexTexture(1.0f, 0.0f);
			
			tiny3d_VertexPos((float)texp->centerX, (float)texp->centerY, z);
			tiny3d_VertexTexture(1.0f, 1.0f);
			
			tiny3d_VertexPos((float)-texp->centerX, (float)texp->centerY, z);
			tiny3d_VertexTexture(0.0f, 1.0f);
		tiny3d_End();	
		
		tiny3d_SetMatrixModelView(NULL); // Identity
	}

	void ya2d_drawBlendTextureZ(const ya2d_Texture *texp, int x, int y, int z, float scale, u32 color)
	{
		if(!texp || !texp->data) return;

		ya2d_setTexture(texp);
		
		tiny3d_SetPolygon(TINY3D_QUADS);
			tiny3d_VertexPos(x, y, z);
			tiny3d_VertexColor(color);
			tiny3d_VertexTexture(0.0f, 0.0f);
			
			tiny3d_VertexPos(x+(texp->imageWidth * scale), y, z);
			tiny3d_VertexTexture(1.0f, 0.0f);
			
			tiny3d_VertexPos(x+(texp->imageWidth * scale), y+(texp->imageHeight * scale), z);
			tiny3d_VertexTexture(1.0f, 1.0f);
			
			tiny3d_VertexPos(x, y+(texp->imageHeight * scale), z);
			tiny3d_VertexTexture(0.0f, 1.0f);
		tiny3d_End();
	}

	ya2d_Texture* ya2d_loadPNGfromFile(const char* filename)
	{
		pngData png;
		pngLoadFromFile(filename, &png);
		if(png.bmp_out)
		{
			ya2d_Texture *texp = ya2d_createTexture(png.width, png.height, TINY3D_TEX_FORMAT_A8R8G8B8);
			memcpy(texp->data, png.bmp_out, texp->dataLength);
			free(png.bmp_out);
			return texp;
		}
		return NULL;
	}

	ya2d_Texture* ya2d_loadPNGfromBuffer(const void *buffer, u32 buf_size)
	{
		pngData png;
		pngLoadFromBuffer(buffer, buf_size, &png);
		if(png.bmp_out)
		{
			ya2d_Texture *texp = ya2d_createTexture(png.width, png.height, TINY3D_TEX_FORMAT_A8R8G8B8);
			memcpy(texp->data, png.bmp_out, texp->dataLength);
			free(png.bmp_out);
			return texp;
		}
		return NULL;
	}

	ya2d_Texture* ya2d_loadJPGfromFile(const char* filename)
	{
		jpgData jpg;
		jpgLoadFromFile(filename, &jpg);
		if(jpg.bmp_out)
		{
			ya2d_Texture *texp = ya2d_createTexture(jpg.width, jpg.height, TINY3D_TEX_FORMAT_A8R8G8B8);
			memcpy(texp->data, jpg.bmp_out, texp->dataLength);
			free(jpg.bmp_out);
			return texp;
		}
		return NULL;
	}

	ya2d_Texture* ya2d_loadJPGfromBuffer(const void *buffer, u32 buf_size)
	{
		jpgData jpg;
		jpgLoadFromBuffer(buffer, buf_size, &jpg);
		if(jpg.bmp_out)
		{
			ya2d_Texture *texp = ya2d_createTexture(jpg.width, jpg.height, TINY3D_TEX_FORMAT_A8R8G8B8);
			memcpy(texp->data, jpg.bmp_out, texp->dataLength);
			free(jpg.bmp_out);
			return texp;
		}
		return NULL;
	}   
