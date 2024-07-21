#include "sensor.h"

#include "json/cJSON.h"

extern "C" {
#include "bsp_USART.h"
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
    memset(m_buf, 0, 256);

    /*    异步串口    */
    uint8_t* rxdata = UART4_GetReceivedData();
    uint16_t rxNum = UART4_GetReceivedNum();
    memcpy(m_buf, rxdata, rxNum < 256 ? rxNum : 256);
    UART4_ClearReceived();

    /*    数据有效    */
    if (m_buf[0] != '\0') {
        cJSON* root = cJSON_Parse(m_buf);
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
        return true;
    }
    return false;
}

void Sensor::update()
{
    /*        更新温湿度和系统工作状态        */
    UART4_SendString(
        "{\"services\": [ {\"service_id\": \"huojia_rfid\", \"properties\":{\"AHT10_T\":%d} }, {\"service_id\": "
        "\"huojia_rfid\", \"properties\":{\"AHT10_H\":%d} }, {\"service_id\": \"huojia_rfid\", "
        "\"properties\":{\"STM32\":%s} }, {\"service_id\": \"huojia_rfid\", \"properties\":{\"Human_reaction\":%s} "
        "}, "
        "{\"service_id\": \"huojia_rfid\", \"properties\":{\"Shelf_light\":%s} }, {\"service_id\": "
        "\"huojia_rfid\", "
        "\"properties\":{\"Shelf_fan\":%s} }, {\"service_id\": \"huojia_rfid\", \"properties\":{\"Humidifier\":%s} "
        "} ] "
        "}",
        (int)m_aht10->get_tem(), (int)m_aht10->get_humi(), m_stm32 ? "true" : "false", m_human ? "true" : "false",
        m_light ? "true" : "false", m_fan ? "true" : "false", m_humi ? "true" : "false");
}