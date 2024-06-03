#ifndef CORE_H
#define CORE_h

#include "screen.h"

class Core
{
public:
    explicit Core(UART_HandleTypeDef* screen_huart, UART_HandleTypeDef* rfid_huart){
        m_screen = new Screen(screen_huart);
    }
private:
    Screen* m_screen;
};

#endif