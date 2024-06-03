#ifndef RFID_H
#define RFID_H

#include "stm32f4xx.h"

#define DATA_GROUP_LEN 19

class Rfid
{
public:
    explicit Rfid()
    {}

    // 主动盘存
    bool read(uint8_t* data, uint16_t size = DATA_GROUP_LEN + 1);
private:
    // UART_HandleTypeDef* m_huart;
};

#endif