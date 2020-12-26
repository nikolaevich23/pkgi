#include "ya2d_controls.h"

void ya2d_controlsRead()
{
	int i;
	ioPadGetInfo2(&ya2d_padinfo);
	for(i = 0; i < MAX_PORT_NUM; i++)
	{		 
		if(ya2d_padinfo.port_status[i])
		{
			ioPadGetData(i, &ya2d_paddata[i]);	 
		}
	}	
}
