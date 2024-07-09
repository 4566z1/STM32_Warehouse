#ifndef _AHT10_H  // 基础文件定义
#define _AHT10_H

#include "common_inc.h"
#include "i2c.h"
#include "stm32f4xx_hal.h"

class AHT10
{
   public:
    explicit AHT10(const int& address) : m_address(address) {}

    const float& get_tem() { return m_tem; }
    const float& get_humi() { return m_tem; }
    const char* get_tem_str() { return m_tem_str; };
    const char* get_humi_str() { return m_humi_str; }
    void init();
    void read();

   private:
    int m_address;
    char m_tem_str[10] = {0};
    char m_humi_str[10] = {0};
    float m_tem = 0.0f;
    float m_humi = 0.0f;
};

#endif
