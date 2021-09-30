#ifndef __IAP_TASK_H__
#define __IAP_TASK_H__

#define LONGVAL_BE(c0,c1,c2,c3) 		(((uint32_t)((uint8_t)(c0))<<24) | (((uint32_t)((uint8_t)(c1)))<<16) | (((uint32_t)((uint8_t)(c2)))<<8) | ((uint32_t)((uint8_t)(c3))))

typedef enum{
	CMD_DATA_IDLE,
	CMD_DATA_ADDR,
	CMD_DATA_BUF,
	CMD_DATA_CHECK,
	CMD_DATA_DONE,
	CMD_DATA_WAIT,
	CMD_DATA_ERR,
}CMD_DATA_STEP;

typedef enum{
	CMD_CTR_IDLE,
	CMD_CTR_INIT,
	CMD_CTR_DONE,
	CMD_CTR_ERR,
}CMD_CTR_STEP;

typedef enum{
	UPDATE_IDLE,
	UPDATE_PRE,
	UPDATE_ING,
	UPDATE_ERR,
}UPDATE_STATUS;


void iap_task_create(void);
void iap_task_delete(void);

#endif
