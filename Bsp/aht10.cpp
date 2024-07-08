#include "aht10.h"

void AHT10::read(float& temperature, float& humidity)
{
    uint8_t sendBuffer[3] = {0xAC, 0x33, 0x00};  // 传感器测量参数
    uint8_t readBuffer[6];
    HAL_I2C_Master_Transmit(&hi2c1, this->m_address, sendBuffer, 3,
                            HAL_MAX_DELAY);  // 调用的指针，从机地址，发送变量存储地址，发送多少位数据，超时时间
    vTaskDelay(75);
    HAL_I2C_Master_Receive(&hi2c1, this->m_address, readBuffer, 6, HAL_MAX_DELAY);

    if ((readBuffer[0] & 0x80) == 0) {  // 确实读取到了数据
        uint32_t data = 0;
        data = (((uint32_t)readBuffer[3] >> 4) + ((uint32_t)readBuffer[2] << 4) + ((uint32_t)readBuffer[1] << 12));
        humidity = data * 100.0f / (1 << 20);  // 根据公式得到湿度
        data = (((uint32_t)readBuffer[3] & 0x0F) << 16) + ((uint32_t)readBuffer[4] << 8) + ((uint32_t)readBuffer[5]);
        temperature = data * 200.f / (1 << 20) - 50;  // 根据公式得到温度
    }
}
