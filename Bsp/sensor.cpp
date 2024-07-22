#include "sensor.h"

#include "json/cJSON.h"

extern "C" {
#include "bsp_USART.h"
#include "usart.h"
}

AHT10 aht10(0x70);

void Sensor::init()
{
    // 温湿度传感器
    aht10.init();
    this->m_aht10 = &aht10;
}

bool Sensor::sync()
{
    /*    异步串口    */
    uint8_t* rxdata = UART4_GetReceivedData();

    /*    数据有效    */
    if (rxdata[0] != '\0') {
        cJSON* root = cJSON_Parse((const char*)rxdata);
        if (root != NULL) {
            cJSON* item;
            item = cJSON_GetObjectItem(root, "temp");
            if (item) {
                m_aht10->get_temres() = item->valueint;
            }
            item = cJSON_GetObjectItem(root, "humi");
            if (item) {
                m_aht10->get_humires() = item->valueint;
            }
            item = cJSON_GetObjectItem(root, "light");
            if (item) {
                m_light = item->valueint;
            }
        }

        UART4_ClearReceived();
        return true;
    }
    return false;
}

void Sensor::update()
{
    /*        更新温湿度和系统工作状态        */
    sprintf(m_buf,
            "{\"services\":[{\"service_id\":\"huojia_rfid\",\"properties\":{\"AHT10_T\":%d}},{\"service_id\":\"huojia_"
            "rfid\",\"properties\":{\"AHT10_H\":%d}},{\"service_id\":\"huojia_rfid\",\"properties\":{\"STM32\":%s}},{"
            "\"service_id\":\"huojia_rfid\",\"properties\":{\"Human_reaction\":%s}},{\"service_id\":\"huojia_rfid\","
            "\"properties\":{\"Shelf_light\":%s}},{\"service_id\":\"huojia_rfid\",\"properties\":{\"Shelf_fan\":%s}},{"
            "\"service_id\":\"huojia_rfid\",\"properties\":{\"Humidifier\":%s}},{\"service_id\":\"huojia_rfid\","
            "\"properties\":{\"Temp_Threshold\":%d}},{\"service_id\":\"huojia_rfid\",\"properties\":{\"Humi_"
            "Threshold\":%d}}]"
            "}",
            (int)aht10.get_tem(), (int)aht10.get_humi(), m_stm32 ? "true" : "false", m_human ? "true" : "false",
            m_light ? "true" : "false", m_fan ? "true" : "false", m_humi ? "true" : "false", aht10.get_temres(),
            aht10.get_humires());

    HAL_UART_Transmit_DMA(&huart4, (const uint8_t*)m_buf, 1024);
}