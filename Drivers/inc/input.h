#ifndef __INPUT_H__
#define __INPUT_H__


enum key_input_e
{
    INPUT_KEY_1 = 0,
    INPUT_KEY_2,
    INPUT_CRASH,
		INPUT_MAX,
};


enum key_action_e
{
    ACTION_HOLD = 0,    
    ACTION_RELEASE,
    ACTION_PRESS,
    ACTION_NONE,
};


void io_input_init(void);
uint8_t io_input_update(uint8_t idx);


#endif
