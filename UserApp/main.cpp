#include "user.h"

BLE ble;
Server server;
Screen screen;
Sensor sensor;

/**
 * @brief 心跳包 Tick（10s）
 *
 */
void tick() { sensor.update(); }

/**
 * @brief RFID Main
 * 
 */
void rfid_main(void)
{
    vTaskDelay(1000);

    while (true) {
        ble.read();

        if (ble.decode()) {
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

    xTimerCreate("tick", (TickType_t)10000, pdTRUE, (void*)1, (TimerCallbackFunction_t)tick);

    while (true) {
        // 同步所有信息
        {
            // 传感器信息
            sensor.sync();
            sensor.m_aht10->read();
            screen.set("sensor", "t2", sensor.m_aht10->get_tem());
            screen.set("sensor", "t3", sensor.m_aht10->get_humi());
            screen.set("sensor", "n0", sensor.m_aht10->get_temres());
            screen.set("sensor", "bt1", sensor.m_light);

            // 仓储信息
            if (server.product_get()) screen.set("ware", "t0", server.get_product());

            vTaskDelay(1);
        }

        // 解析屏幕指令 (由于串口是异步的，不能及时分清楚数据是哪个，只能粗略通过比较大小判断)
        {
            screen.get("sensor", "n0");
            screen.get("sensor", "bt1");

            char header = screen.get_data()[0];
            char value = screen.get_data()[1];
            if (header == 0x71) {
                if (value > 1) {
                    // value 为 当前温度阈值
                    LOG("screen_main => open temp: %d\n", value);
                    (int)sensor.m_aht10->get_tem() > value ? HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET)
                                                           : HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);

                } else {
                    // value 为 当前开灯状态
                    LOG("screen_main => open light: %d\n", value);
                    value == 0x01 ? HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET)
                                  : HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
                }
            }
            vTaskDelay(1);
        }
        vTaskDelay(100);
    }
}