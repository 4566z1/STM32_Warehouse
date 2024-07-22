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
            // sensor.update();
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
            // LOG("rfid_main => name: %s code: %s mode: %s\r\n", ble.get()->name, ble.get()->code, ble.get()->mode);

            int mode = atoi(ble.get()->mode);
            !mode ? server.product_add(ble.get()->name, ble.get()->code) : server.product_del(ble.get()->name);
        }

        vTaskDelay(100);
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
            // 本地同步
            sensor.m_aht10->read();
            screen.set("sensor", "t2", sensor.m_aht10->get_tem_str());
            screen.set("sensor", "t3", sensor.m_aht10->get_humi_str());

            // 云端同步
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
            char header = screen.get_data()[0];
            char value = screen.get_data()[4];
            if (header == 0x06) {
                sensor.m_aht10->get_temres() = value;
            } else if (header == 0x0B) {
                sensor.m_aht10->get_humires() = value;
            } else if (header == 0x0A) {
                sensor.m_light = value;
            }

            vTaskDelay(1);
        }

        // 控制器
        {
            /*    温控    */
            (int)sensor.m_aht10->get_tem() > sensor.m_aht10->get_humires()
                ? HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET)
                : HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);

            /*    灯光     */
            sensor.m_light ? HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET)
                           : HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);

            /*  人体传感器  */
            sensor.m_human = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8);

            /*    加湿器    */
        }
        vTaskDelay(100);
    }
}