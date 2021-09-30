#include "arch_define.h"
#include "arch_os.h"

list_head_t async_list = {0};

void arch_os_mutex_get(arch_os_mutex* mutex)
{
#if (defined(USER_OS_ENABLE) && USER_OS_ENABLE)
	xSemaphoreTake(*mutex, portMAX_DELAY);
#endif
}

void arch_os_mutex_put(arch_os_mutex* mutex)
{
#if (defined(USER_OS_ENABLE) && USER_OS_ENABLE)
	xSemaphoreGive(*mutex);
#endif
}

void arch_os_mutex_init(arch_os_mutex* mutex)
{
#if (defined(USER_OS_ENABLE) && USER_OS_ENABLE)
	*mutex = xSemaphoreCreateMutex();
#endif
}

void arch_os_mutex_deinit(arch_os_mutex *mutex)
{
#if (defined(USER_OS_ENABLE) && USER_OS_ENABLE)
    if((*mutex) != NULL)
    {
        vSemaphoreDelete(*mutex);
        *mutex = NULL;
    }
#endif
}

void arch_os_async_init()
{
	INIT_LIST_HEAD(&async_list);
}

void arch_os_async_call(void(* callback)(void *), void *value, unsigned int delay)
{
    os_async_callback_t *async_cb = arch_malloc(sizeof(os_async_callback_t));
	if (NULL == async_cb) {
		return;
	}

    async_cb->cb = callback;
    async_cb->value = value;
    async_cb->time = millis() + delay;

	list_add(&async_cb->list, &async_list);   
}

void arch_os_async_proc()
{
	struct list_head *pos, *q;    
    os_async_callback_t *tmp = NULL;
    uint32_t curr_time = millis();

	list_for_each_safe(pos, q, &async_list) {
		tmp = list_entry(pos, os_async_callback_t, list);
		if (curr_time > tmp->time) {
			tmp->cb(tmp->value);
			list_del(pos);
			arch_free(tmp);
            tmp = NULL;			
		}
	}
}
