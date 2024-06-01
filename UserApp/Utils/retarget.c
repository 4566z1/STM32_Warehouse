#include "retarget.h"

#include "usart.h"

#if !defined(OS_USE_SEMIHOSTING)

#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

UART_HandleTypeDef *gHuart;

void RetargetInit(UART_HandleTypeDef *huart){
    gHuart = huart;

    /* Disable I/O buffering for STDOUT stream, so that
     * chars are sent out as soon as they are printed. */
    setvbuf(stdout, NULL, _IONBF, 0);
}

int _write(int fd, char *ptr, int len)
{
    if (fd == STDOUT_FILENO || fd == STDERR_FILENO)
    {
        HAL_UART_Transmit(gHuart, (uint8_t *) ptr, len, HAL_MAX_DELAY);
        return len;
    } else
        return -1;
}

#endif //#if !defined(OS_USE_SEMIHOSTING)