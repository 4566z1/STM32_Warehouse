#ifndef _AHT10_H  // 基础文件定义
#define _AHT10_H

#include "common_inc.h"
#include "i2c.h"
#include "stm32f4xx_hal.h"

class AHT10
{
   public:
    explicit AHT10(const int& address) : m_address(address) {}

    const bool& is_init() { return m_is_init; }
    const float& get_tem() { return m_tem; }
    const float& get_humi() { return m_humi; }
    int& get_temres() { return m_temp_res; }
    int& get_humires() { return m_humi_res; }
    const char* get_tem_str() { return m_tem_str; };
    const char* get_humi_str() { return m_humi_str; }
    
    void init();
    void read();

   private:
    bool m_is_init = false;
    int m_address;

    char m_tem_str[10] = {0};
    char m_humi_str[10] = {0};
    int m_temp_res = 35;
    int m_humi_res = 60;
    float m_tem = 0.0f;
    float m_humi = 0.0f;
};

#endif
