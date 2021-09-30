
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "timer.h"
#include "output.h"
#include "usart.h"
#include "flash.h"
#include "ota.h"
#include "iap_task.h"


#define ADDR_SIZE			4

#define IAP_TASK_PRIO		5
#define IAP_STK_SIZE 		256


typedef struct _iap_operation{
	UPDATE_STATUS status;	
	CMD_CTR_STEP ctrl_flag;
	CMD_DATA_STEP data_flag;
	uint16_t addr_cnt;
	uint16_t data_cnt;
	uint8_t *addr_buf;
	uint8_t *data_buf;
	uint32_t checksum;
}iap_operation_t;


static TaskHandle_t iap_task_handler = NULL;
static SemaphoreHandle_t xSemaphorIAPHdl = NULL;
static uint8_t recv_data;


static uint8_t uart1_receive_callback(uint8_t _input)
{
    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

    if(xSemaphorIAPHdl == NULL)
        return 0;
    
    xSemaphoreGiveFromISR(xSemaphorIAPHdl, &xHigherPriorityTaskWoken);
    portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
		recv_data = _input;
	return 0;
}


static void dbg_led_blink(void)
{
	io_output_toggle(OUTPUT_LED_DBG);
}


static inline void ACK_OK(void)
{
    uart1_send(0xCC);
    uart1_send(0xDD);
}


static inline void ACK_ERR(void)
{
    uart1_send(0xEE);
    uart1_send(0xFF);
}


static uint8_t iap_next_status(uint8_t status)
{
	(status == UPDATE_ERR) ? ACK_ERR() : ACK_OK();
	return status;
}


static void iap_reset_status(iap_operation_t *iap)
{
	iap->status = UPDATE_IDLE;
	iap->ctrl_flag = CMD_CTR_IDLE;
	iap->data_flag = CMD_DATA_IDLE;
	iap->addr_cnt = 0;
	iap->data_cnt = 0;
}


void iap_upgrade_handle(iap_operation_t *iap, uint8_t _input)
{
	uint8_t status_temp = (uint8_t)iap->status;
		
	switch(status_temp){
			
		case UPDATE_IDLE:
			if(iap->ctrl_flag == CMD_CTR_IDLE){
				if(_input == 0x5A)
					iap->ctrl_flag = CMD_CTR_INIT;
			}else if(iap->ctrl_flag == CMD_CTR_INIT){
				if(_input == 0xA5){
					timer5_setperiod(500);
					iap->ctrl_flag = CMD_CTR_IDLE;
					status_temp = iap_next_status(UPDATE_PRE);
				}else
					status_temp = iap_next_status(UPDATE_ERR);
			}
			break;

		case UPDATE_PRE:
			if(iap->ctrl_flag == CMD_CTR_IDLE){
				if(_input == 0x5A)
					iap->ctrl_flag = CMD_CTR_INIT;
				else
					iap->ctrl_flag = iap_next_status(UPDATE_ERR);
			}else if(iap->ctrl_flag == CMD_CTR_INIT){
				if(_input == 0x01){
					iap->ctrl_flag = CMD_CTR_IDLE;
					status_temp = iap_next_status(UPDATE_ING);
				}else
					status_temp = iap_next_status(UPDATE_ERR);
			}
			break;

		case UPDATE_ING:
			if(iap->data_flag == CMD_DATA_IDLE) {
				if(_input == 0x31){
					iap->addr_cnt = 0;
					iap->checksum = 0;
					iap->data_flag = CMD_DATA_ADDR;
				}else
					status_temp = iap_next_status(UPDATE_ERR);
			}else if(iap->data_flag == CMD_DATA_ADDR) {
				iap->addr_buf[iap->addr_cnt++] = _input;
				iap->checksum += _input;
				if(iap->addr_cnt >= ADDR_SIZE){
					iap->data_cnt = 0;
					iap->data_flag = CMD_DATA_BUF;
				}
			}else if(iap->data_flag == CMD_DATA_BUF) {
				iap->data_buf[iap->data_cnt++] = _input;
				iap->checksum += _input;
				if(iap->data_cnt >= PAGE_SIZE){
					iap->data_flag = CMD_DATA_CHECK;
				}
			}else if(iap->data_flag == CMD_DATA_CHECK) {
				if(_input != (iap->checksum & 0xFF))
					status_temp = iap_next_status(UPDATE_ERR);
				else{
					flash_write_bytes(LONGVAL_BE(iap->addr_buf[0], iap->addr_buf[1], iap->addr_buf[2], iap->addr_buf[3]), 
									iap->data_buf, iap->data_cnt);
					ACK_OK();
					iap->data_flag = CMD_DATA_DONE;
				}
			}else if(iap->data_flag == CMD_DATA_DONE) {
				if(_input == 0x31){
					iap->addr_cnt = 0;
					iap->data_cnt = 0;
					iap->checksum = 0;
					iap->data_flag = CMD_DATA_ADDR;
				}else if(iap->ctrl_flag == CMD_CTR_IDLE) {
					if(_input == 0x5A)
						iap->ctrl_flag = CMD_CTR_INIT;
					else
						status_temp = iap_next_status(UPDATE_ERR);
				}else if(iap->ctrl_flag == CMD_CTR_INIT) {
					if(_input == 0x02){
						status_temp = iap_next_status(UPDATE_IDLE);
						ota_update_app();
						NVIC_SystemReset();
					}
					else
						status_temp = iap_next_status(UPDATE_ERR);
				}
			}else if(iap->data_flag == CMD_DATA_ERR)
				status_temp = iap_next_status(UPDATE_ERR);
			break;
	}
	iap->status = (UPDATE_STATUS)status_temp;
}




static void iap_task(void *pvParameters)
{
	iap_operation_t *iap_op = NULL;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPERIPH_FLASH, ENABLE);
	io_output_init();
	timer5_init(10000, dbg_led_blink);
    uart1_init(115200, uart1_receive_callback);	
	xSemaphorIAPHdl = xSemaphoreCreateBinary();

	iap_op = (iap_operation_t *)calloc(1, sizeof(iap_operation_t));
	iap_op->addr_buf = (uint8_t *)calloc(ADDR_SIZE, sizeof(uint8_t));
	iap_op->data_buf = (uint8_t *)calloc(PAGE_SIZE, sizeof(uint8_t));

	iap_reset_status(iap_op);

	while(1){
        if(xSemaphoreTake(xSemaphorIAPHdl, 3000 / portTICK_PERIOD_MS) == pdTRUE){	
            
            iap_upgrade_handle(iap_op, recv_data);
            
        }else{
            iap_reset_status(iap_op);
			timer5_setperiod(10000);
  			//if(ota_valid_app() > 0)
				ota_run_app();
        }
	}

}   


void iap_task_create(void)
{
    xTaskCreate((TaskFunction_t )iap_task,     
                (const char*    )"iap_task",   
                (uint16_t       )IAP_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )IAP_TASK_PRIO,
                (TaskHandle_t*  )&iap_task_handler);  

}



