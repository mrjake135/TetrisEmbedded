#ifndef __APP_DBG_H__
#define __APP_DBG_H__

#if defined(STM32L_PLATFORM_BOOT)
#include "../common/xprintf.h"

#if defined(APP_DBG_EN)
#define APP_DBG(fmt, ...)       xprintf((const char*)fmt, ##__VA_ARGS__)
#else
#define APP_DBG(fmt, ...)
#endif

#if defined(APP_PRINT_EN)
#define APP_PRINT(fmt, ...)       xprintf((const char*)fmt, ##__VA_ARGS__)
#else
#define APP_PRINT(fmt, ...)
#endif
#endif

#endif //__APP_DBG_H__
