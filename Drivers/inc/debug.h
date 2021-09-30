#ifndef __DEBUG_H
#define __DEBUG_H	 

#include <stdio.h>

#define LOG_LEVEL_VERBOSE		0
#define LOG_LEVEL_INFO			1
#define LOG_LEVEL_WARNING		2
#define LOG_LEVEL_ERROR			3
#define LOG_LEVEL_FATAL			4
#define LOG_LEVEL_OFF			5

typedef int log_level_t;

#define debug_printf				printf
void debug_printf_init(unsigned int bound);
void debug_printf_begin(void);
void debug_printf_end(void);

#ifndef RK_LOG_LEVEL
#define RK_LOG_LEVEL			LOG_LEVEL_VERBOSE
#endif

#define LOG_LEVEL				RK_LOG_LEVEL

#if RK_LOG_LEVEL <= LOG_LEVEL_INFO
#define LOG_INFO(_fmt_, ...) \
	do{\
		extern log_level_t g_log_level;\
		if(g_log_level <= LOG_LEVEL_INFO){\
			debug_printf_begin();\
			debug_printf("[I] "_fmt_, ##__VA_ARGS__);\
			debug_printf_end();\
		}\
	}while(0)

#define LOG_INFO_TAG(TAG, _fmt_, ...) \
	do{\
		if((LOG_LEVEL) <= LOG_LEVEL_INFO){\
			debug_printf_begin();\
			debug_printf("[I] %s: "_fmt_, TAG, ##__VA_ARGS__);\
			debug_printf_end();\
		}\
	}while(0)

#else

#define LOG_INFO(_fmt_, ...) \
	do{}while(0)

#define LOG_INFO_TAG(TAG, _fmt_, ...) \
	do{}while(0)

#endif

#if RK_LOG_LEVEL <= LOG_LEVEL_WARNING
#define LOG_WARN(_fmt_, ...) \
	do{\
		extern log_level_t g_log_level;\
		if(g_log_level <= LOG_LEVEL_WARNING){\
			debug_printf_begin();\
			debug_printf("[W] "_fmt_" (%s, %d, %s)", ##__VA_ARGS__, __FILE__, __LINE__, __FUNCTION__);\
			debug_printf_end();\
		}\
	}while(0)
#define LOG_WARN_TAG(TAG, _fmt_, ...) \
	do{\
		if((LOG_LEVEL) <= LOG_LEVEL_WARNING){\
			debug_printf_begin();\
			debug_printf("[W] %s: "_fmt_" (%s, %d, %s)", TAG, ##__VA_ARGS__, __FILE__, __LINE__, __FUNCTION__);\
			debug_printf_end();\
		}\
	}while(0)
#else
#define LOG_WARN(_fmt_, ...) \
	do{}while(0)
#define LOG_WARN_TAG(TAG, _fmt_, ...) \
	do{}while(0)
#endif

#if RK_LOG_LEVEL <= LOG_LEVEL_ERROR
#define LOG_ERROR(_fmt_, ...) \
	do{\
		extern log_level_t g_log_level;\
		if(g_log_level <= LOG_LEVEL_ERROR){\
			debug_printf_begin();\
			debug_printf("[E] "_fmt_" (%s, %s, %d)", ##__VA_ARGS__, __FILE__, __FUNCTION__, __LINE__);\
			debug_printf_end();\
		}\
	}while(0)
#define LOG_ERROR_TAG(TAG, _fmt_, ...) \
	do{\
		if((LOG_LEVEL) <= LOG_LEVEL_ERROR){\
			debug_printf_begin();\
			debug_printf("[E] %s: "_fmt_" (%s, %s, %d)", TAG, ##__VA_ARGS__, __FILE__, __FUNCTION__, __LINE__);\
			debug_printf_end();\
		}\
	}while(0)
#else
#define LOG_ERROR(_fmt_, ...) \
	do{}while(0)
#define LOG_ERROR_TAG(TAG, _fmt_, ...) \
	do{}while(0)
#endif

#if RK_LOG_LEVEL <= LOG_LEVEL_FATAL
#define LOG_FATAL(_fmt_, ...) \
	do{\
		extern log_level_t g_log_level;\
		if(g_log_level <= LOG_LEVEL_FATAL){\
			debug_printf_begin();\
			debug_printf("[F] "_fmt_" (%s, %d)", ##__VA_ARGS__, __FUNCTION__, __LINE__);\
			debug_printf_end();\
		}\
	}while(0)
#define LOG_FATAL_TAG(TAG, _fmt_, ...) \
	do{\
		if((LOG_LEVEL) <= LOG_LEVEL_FATAL){\
			debug_printf_begin();\
			debug_printf("[F] %s: "_fmt_" (%s, %d)", TAG, ##__VA_ARGS__, __FUNCTION__, __LINE__);\
			debug_printf_end();\
		}\
	}while(0)

#else
#define LOG_FATAL(_fmt_, ...) \
	do{}while(0)
#define LOG_FATAL_TAG(TAG, _fmt_, ...) \
	do{}while(0)
#endif

#if RK_LOG_LEVEL <= LOG_LEVEL_VERBOSE
#define LOG_DEBUG(_fmt_, ...) \
	do{\
		extern log_level_t g_log_level;\
		if(g_log_level <= LOG_LEVEL_VERBOSE){\
			debug_printf_begin();\
			debug_printf("[D] "_fmt_, ##__VA_ARGS__);\
			debug_printf_end();\
		}\
	}while(0)
#define LOG_DEBUG_TAG(TAG, _fmt_, ...) \
	do{\
		if((LOG_LEVEL) <= LOG_LEVEL_VERBOSE){\
			debug_printf_begin();\
			debug_printf("[D] %s: "_fmt_, TAG, ##__VA_ARGS__);\
			debug_printf_end();\
		}\
	}while(0)
#else
#define LOG_DEBUG(_fmt_, ...) \
	do{}while(0)
#define LOG_DEBUG_TAG(TAG, _fmt_, ...) \
	do{}while(0)
#endif


#endif

