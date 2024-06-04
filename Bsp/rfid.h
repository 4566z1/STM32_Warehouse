#ifndef RFID_H
#define RFID_H

#include "stm32f4xx.h"

const int DATA_GROUP_LEN = 19;

class Rfid
{
public:
    explicit Rfid()
    {}

    // 主动盘存，读DATA_GROUP_LEN长度
    bool read(char* data);
private:

};

#endif