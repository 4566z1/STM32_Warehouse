#ifndef _USER_H_
#define _USER_H_

#include "bluetooth.h"
#include "common_inc.h"
#include "screen.h"
#include "server.h"
#include "sensor.h"

extern "C" {
#include "bsp_USART.h"
}

/* 调试模式 */
#define MAIN_DEBUG

#ifdef MAIN_DEBUG
#define LOG USART1_SendString
#else
#define LOG  //
#endif

/** Implement C++ new/delete operators using the heap  */
void* operator new(size_t size) //重载全局operator new
{
	return pvPortMalloc(size);
}
void* operator new[](size_t size)//重载全局operator new[]
{
	return pvPortMalloc(size);
}
void operator delete[](void* phead)//重载全局operator delete[]
{
	vPortFree(phead);
}
void operator delete(void* phead)//重载全局operator delete
{
	vPortFree(phead);
}
void operator delete(void* phead, unsigned int)
{
	vPortFree(phead);
}
void operator delete[](void* phead, unsigned int)
{
	vPortFree(phead);
}
#endif 