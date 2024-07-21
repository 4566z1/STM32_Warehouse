#ifndef _SENSOR_H_
#define _SENSOR_H_

#include "aht10.h"

class Sensor
{
   public:
    Sensor() {}

    /**
     * @brief  初始化传感器
     *
     */
    void init();

    /**
     * @brief 本地上传云端
     *
     */
    void update();

    /**
     * @brief 本地同步云端数据
     *
     */
    void sync();

    AHT10 *m_aht10;
    bool m_light = false;
    bool m_fan = false;
    bool m_humi = false;
    bool m_human = false;
    bool m_stm32 = true;

   private:
    char m_buf[256] = {0};
};

#endif