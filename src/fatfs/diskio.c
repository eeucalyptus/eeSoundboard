#include "ff.h"
#include "diskio.h"

#include <hw/Flash.h>


DSTATUS disk_status (BYTE pdrv)
{
	return 0; // Assume device as initialized
}


DSTATUS disk_initialize (BYTE pdrv)
{
	return 0; // Initialization is done elsewhere
}


DRESULT disk_read (BYTE pdrv, BYTE *buff, LBA_t sector,	UINT count)
{
	// TODO update for 512 B blocksize

	Flash_StartRead(sector*4096);
	for(int i = 0; i < count*4096; i++) {
		*(buff++) = Flash_Read();
	}
	Flash_StopRead();

	return 0;
}
