#ifndef __OUTPUT_H__
#define __OUTPUT_H__

enum io_output_e
{
    OUTPUT_POWER_3V = 0,
    OUTPUT_LED_1,
    OUTPUT_LED_2,
    OUTPUT_LED_3,
		OUTPUT_LED_4,
    OUTPUT_LED_BL,
    OUTPUT_LED_DBG,
    OUTPUT_ESP_RST,  
		OUTPUT_MAX,
};

extern void io_output_init(void);
extern void io_output_set(uint8_t idx, uint8_t value);
extern void io_output_toggle(uint8_t idx);
extern uint8_t io_output_get(uint8_t idx);

#endif
