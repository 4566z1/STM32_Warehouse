#ifndef RETARGET_H
#define RETARGET_H

#include "stm32f4xx_hal.h"
#include <stdio.h>

void RetargetInit(UART_HandleTypeDef *huart);

int _write(int fd, char *ptr, int len);

#endif