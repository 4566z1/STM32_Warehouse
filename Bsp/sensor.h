#ifndef _SENSOR_H_
#define _SENSOR_H_

#include "aht10.h"

class Sensor
{
   public:
    Sensor() {}

    /**
     * @brief 是否有效
     *
     * @return true
     * @return false
     */
    bool is_ok() { return m_aht10->is_init(); }

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
    bool sync();

    AHT10 *m_aht10;
    bool m_is_init = false;
    bool m_light = false;
    bool m_fan = false;
    bool m_humi = false;
    bool m_human = false;
    bool m_stm32 = true;

   private:
    char m_buf[1024] = {0};
};

#endif