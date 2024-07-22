#include "aht10.h"

#include "stdio.h"
#include "string.h"

void AHT10::init()
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

void AHT10::read()
{
    memset(m_tem_str, 0, 10);
    memset(m_humi_str, 0, 10);
    
    uint8_t sendBuffer[3] = {0xAC, 0x33, 0x00};  // 传感器测量参数
    uint8_t readBuffer[6];
    HAL_I2C_Master_Transmit(&hi2c1, this->m_address, sendBuffer, 3,
                            HAL_MAX_DELAY);  // 调用的指针，从机地址，发送变量存储地址，发送多少位数据，超时时间
    vTaskDelay(75);
    HAL_I2C_Master_Receive(&hi2c1, this->m_address, readBuffer, 6, HAL_MAX_DELAY);

    if ((readBuffer[0] & 0x80) == 0) {  // 确实读取到了数据
        uint32_t data = 0;
        data = (((uint32_t)readBuffer[3] >> 4) + ((uint32_t)readBuffer[2] << 4) + ((uint32_t)readBuffer[1] << 12));
        this->m_humi = data * 100.0f / (1 << 20);  // 根据公式得到湿度
        data = (((uint32_t)readBuffer[3] & 0x0F) << 16) + ((uint32_t)readBuffer[4] << 8) + ((uint32_t)readBuffer[5]);
        this->m_tem = data * 200.f / (1 << 20) - 50;  // 根据公式得到温度
    }

    if(!m_is_init) m_is_init = true;
    sprintf(this->m_tem_str, "%d", (int)this->m_tem);
    sprintf(this->m_humi_str, "%d", (int)this->m_humi);
}
