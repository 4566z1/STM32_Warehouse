#include "sensor.h"

#include "json/json.h"

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

void Sensor::sync()
{
    char buf[256] = {0};

    /*    异步串口    */
    uint8_t* rxdata = UART4_GetReceivedData();
    uint16_t rxNum = UART4_GetReceivedNum();
    memcpy(buf, rxdata, rxNum < 256 ? rxNum : 256);
    UART4_ClearReceived();

    /*    数据有效    */
    if (buf[0] != '\0') {
        Json::Value root(buf);

        /*      更新货架灯      */
        if (root["command_name"] == "light") {
            Json::Value light_state = root["paras"]["light_state"];
            if (!light_state.empty()) {
                m_light = light_state.asBool();
            }
        }
    }
}

void Sensor::update()
{
    /*        更新温湿度和系统工作状态        */
    UART4_SendString(
        "{\"services\": [ {\"service_id\": \"huojia_rfid\", \"properties\":{\"AHT10_T\":%2.2lf} }, "
        "{\"service_id\": \"huojia_rfid\", \"properties\":{\"AHT10_H\":%2.2lf} }, {\"service_id\": "
        "\"huojia_rfid\", \"properties\":{\"STM32\":%s} } ] }",
        m_aht10->get_tem(), m_aht10->get_humi(), m_stm32 ? "true" : "false");

    /*        更新货架灯        */
    UART4_SendString("{\"services\": [ {\"service_id\": \"huojia_rfid\", \"properties\":{\"Shelf_light\":%s} } ] }",
                     m_light ? "true" : "false");

    /*        更新人体红外传感器        */
    UART4_SendString("{\"services\": [ {\"service_id\": \"huojia_rfid\", \"properties\":{\"Human_reaction\":%s} } ] }",
                     m_human ? "true" : "false");

    /*        更新加湿器工作状态        */
    UART4_SendString("{\"services\": [ {\"service_id\": \"huojia_rfid\", \"properties\":{\"Humidifier\":%s} } ] }",
                     m_humi ? "true" : "false");

    /*        温湿度阈值信息更新        */
    UART4_SendString(
        "{\"services\": [ {\"service_id\": \"huojia_rfid\", \"properties\":{\"Temp_Threshold\":%d} },{\"service_id\": "
        "\"huojia_rfid\", \"properties\":{\"Humi_Threshold\":%d} } ] }",
        m_aht10->get_temres(), m_aht10->get_humires());

    /*        温湿度阈值报警        */
    UART4_SendString(
        "{\"services\": [ {\"service_id\": \"huojia_rfid\", \"properties\":{\"Warning_Temp\":%s} },{\"service_id\": "
        "\"huojia_rfid\", \"properties\":{\"Warning_Humi\":%s} } ] }",
        m_aht10->get_tem() > m_aht10->get_temres() ? "\"Attention\"" : "\"Normal\"",
        m_aht10->get_humi() > m_aht10->get_humires() ? "\"Attention\"" : "\"Normal\"");
}