#ifndef _AHT10_H  // 基础文件定义
#define _AHT10_H

#include "common_inc.h"
#include "i2c.h"
#include "stm32f4xx_hal.h"

class AHT10
{
   public:
    explicit AHT10(const int& address) : m_address(address)
    {
        uint8_t readBuffer;
        HAL_I2C_Master_Receive(&hi2c1, this->m_address, &readBuffer, 1,
                               HAL_MAX_DELAY);  // 调用的指针，从机地址，读取变量存储地址，读取多少位数据，超时时间
        if ((readBuffer & 0x08) == 0x00) {                // 从机返回值不为1
            uint8_t send_Buffer[3] = {0xE1, 0x08, 0x00};  // 初始化传感器参数
            HAL_I2C_Master_Transmit(&hi2c1, this->m_address, send_Buffer, 3,
                                    HAL_MAX_DELAY);  // 调用的指针，从机地址，发送变量存储地址，发送多少位数据，超时时间
        }
    }
    
    void read(float& temperature, float& humidity);

   private:
    int m_address;
};

#endif
