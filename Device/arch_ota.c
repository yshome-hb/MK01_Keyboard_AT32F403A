#include "miio_define.h"
#include "arch_ota.h"

int arch_ota_func(unsigned char *pbuf, size_t len, size_t sum)
{
	/* trans data to MCU flash here */
	return MIIO_OK;
}
