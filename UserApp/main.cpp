#include "user.h"

BLE ble;
Server server;
Screen screen;
Sensor sensor;

/**
 * @brief 心跳包 Tick（10s）
 *
 */
void tick()
{
    vTaskDelay(1000);

    while (true) {
        if (sensor.is_ok()) {
            sensor.update();
        }
        vTaskDelay(10000);
    }
}

/**
 * @brief RFID Main
 *
 */
void rfid_main(void)
{
    vTaskDelay(1000);

    while (true) {
        if (ble.read()) {
            int mode = atoi(ble.get()->mode);
            !mode ? server.product_add(ble.get()->name, ble.get()->code) : server.product_del(ble.get()->name);
        }

        vTaskDelay(50);
    }
}

/**
 * @brief Screen Main
 *
 */
void screen_main(void)
{
    vTaskDelay(1000);
    sensor.init();
    screen.init();

    while (true) {
        // 同步信息
        {
            // 从本地同步
            sensor.m_aht10->read();
            screen.set("sensor", "t2", sensor.m_aht10->get_tem_str());
            screen.set("sensor", "t3", sensor.m_aht10->get_humi_str());

            // 从云端同步
            if (sensor.sync()) {
                screen.set("sensor", "bt1", sensor.m_light);
                screen.set("sensor", "n0", sensor.m_aht10->get_temres());
                screen.set("sensor", "n1", sensor.m_aht10->get_humires());
            }
            if (server.has_data()) {
                screen.set("ware", "t0", server.get_data());
            }

            vTaskDelay(1);
        }

        // 解析屏幕信息
        {
            if (screen.has_data()) {
                const char* data = screen.get_data();
                sensor.m_aht10->get_temres() = data[0];
                sensor.m_aht10->get_humires() = data[1];
                sensor.m_light = data[2];
            }
            vTaskDelay(1);
        }

        // 控制器
        {
            /*    温控    */
            (int)sensor.m_aht10->get_tem() > sensor.m_aht10->get_temres()
                ? HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET)
                : HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);

            /*    灯光     */
            sensor.m_light ? HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET)
                           : HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);

            /*  人体传感器  */
            sensor.m_human = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8);

            /*    加湿器    */
            sensor.m_aht10->get_humi() < sensor.m_aht10->get_humires() ? sensor.m_humi = true : sensor.m_humi = false;
            if (sensor.m_humi_last != sensor.m_humi) {
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
                vTaskDelay(20);
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
            }
            sensor.m_humi_last = sensor.m_humi;
        }
        vTaskDelay(50);
    }
}