#include "ya2d_draw.h"




void ya2d_drawRectZ(int x, int y, int z, int w, int h, u32 color)
{
	tiny3d_SetPolygon(TINY3D_LINE_LOOP);
		tiny3d_VertexPos(x, y, z);
		tiny3d_VertexColor(color);
		tiny3d_VertexPos(x, y+h, z);
		tiny3d_VertexPos(x+w, y+h, z);
		tiny3d_VertexPos(x+w, y, z);
	tiny3d_End();	
}

void ya2d_drawFillRectZ(int x, int y, int z, int w, int h, u32 color)
{
	tiny3d_SetPolygon(TINY3D_TRIANGLE_STRIP);
		tiny3d_VertexPos(x, y, z);
		tiny3d_VertexColor(color);
		tiny3d_VertexPos(x, y+h, z);
		tiny3d_VertexPos(x+w, y, z);
		tiny3d_VertexPos(x+w, y+h, z);
	tiny3d_End();
}
