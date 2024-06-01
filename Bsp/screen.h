#ifndef SCREEN_H
#define SCREEN_H

#include "stm32f4xx.h"

class Screen{
public:
    explicit Screen(UART_HandleTypeDef* huart)
        : m_huart(huart)
    {};

private:
    UART_HandleTypeDef* m_huart;
};

#endif