#include "ya2d_screen.h"

static long  ya2d_frameCount = 0;
static s64   ya2d_currentTime = 0, ya2d_lastTime = 0, ya2d_deltaTime = 0;
static float ya2d_FPS = 0.0f;
static u32   ya2d_clearColor = 0x0;

//Screen

	void ya2d_screenClear()
	{
		tiny3d_Clear(ya2d_clearColor, TINY3D_CLEAR_ALL);

		// Enable alpha blending.
		tiny3d_BlendFunc(1, TINY3D_BLEND_FUNC_SRC_RGB_SRC_ALPHA | TINY3D_BLEND_FUNC_SRC_ALPHA_SRC_ALPHA,
							TINY3D_BLEND_FUNC_DST_RGB_ONE_MINUS_SRC_ALPHA | TINY3D_BLEND_FUNC_DST_ALPHA_ZERO,
							TINY3D_BLEND_RGB_FUNC_ADD | TINY3D_BLEND_ALPHA_FUNC_ADD);
							
	}
	
	void ya2d_screenSetClearColor(u32 new_color)
	{
		ya2d_clearColor = new_color;
	}
	
	void ya2d_screenBeginDrawing()
	{
		tiny3d_Project2D();
	}

	void ya2d_screenFlip()
	{
		tiny3d_Flip();
        //FPS counting
			ya2d_frameCount++;
			ya2d_currentTime = ya2d_millis();
			ya2d_deltaTime = ya2d_currentTime - ya2d_lastTime;
			if(ya2d_deltaTime >= 1000)
			{
				ya2d_FPS = (float)ya2d_frameCount/((float)ya2d_deltaTime/1000.0f);
				ya2d_frameCount = 0;
				ya2d_lastTime = ya2d_millis();
			}
	}
	
    float ya2d_screenFPS()
    {
		return ya2d_FPS;
	}
