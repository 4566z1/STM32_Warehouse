#include "aht10.h"
#include "bluetooth.h"
#include "common_inc.h"
#include "screen.h"
#include "server.h"

extern "C" {
#include "bsp_USART.h"
}

/* 调试模式 */
//#define MAIN_DEBUG

#ifdef MAIN_DEBUG
#define LOG USART1_SendString
#else
#define LOG  //
#endif

AHT10 aht10(0x70);
BLE ble;
Server server;
Screen screen;

// RFID program entry
void rfid_main(void)
{
    vTaskDelay(1000);

    while (true) {
        ble.read();

        if (ble.decode()) {
            LOG("rfid_main => name: %s code: %s mode: %s\r\n", ble.get()->name, ble.get()->code, ble.get()->mode);

            int mode = atoi(ble.get()->mode);
            if (mode == 0)
                server.product_add(ble.get()->name, ble.get()->code);
            else
                server.product_del(ble.get()->name);

        } else {
            LOG("rfid_main => %s", "Bluetooth is no data.\n");
        }

        vTaskDelay(100);
    }
}

// Screen program entry
void screen_main(void)
{
    vTaskDelay(1000);
    aht10.init();
    screen.init();

    while (true) {
        // Temperature && humidity
        {
            aht10.read();

            // LOG("screen_main => temp_str: %s\n", aht10.get_tem_str());
            // LOG("screen_main => temp_str: %s\n", aht10.get_humi_str());

            screen.send_command("sensor.t2.txt=\"");
            screen.send_command(aht10.get_tem_str());
            screen.send_command("\"\xff\xff\xff");

            screen.send_command("sensor.t3.txt=\"");
            screen.send_command(aht10.get_humi_str());
            screen.send_command("\"\xff\xff\xff");
            vTaskDelay(1);
        }

        // Warehouse data
        {
            if (server.product_get()) {
                screen.send_command("ware.t0.txt=\"");
                screen.send_command(server.get_product());
                screen.send_command("\"\xff\xff\xff");
            }
            vTaskDelay(1);
        }

        // Controller
        {
            char buf[20] = {0};
            screen.send_command("get sensor.n0.val\xff\xff\xff");
            screen.send_command("get sensor.bt1.val\xff\xff\xff");

            if (screen.get_command(buf, 20)) {
                if (buf[1] > 1) {
                    if ((int)aht10.get_tem() > buf[1]) {
                        LOG("screen_main => open temp: %d\n", buf[1]);
                        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
                    } else {
                        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
                    }
                } else {
                    if (buf[1] == 0x01) {
                        LOG("screen_main => open light: %d\n", buf[1]);
                        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
                    } else {
                        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
                    }
                }
            }
            vTaskDelay(1);
        }
        vTaskDelay(100);
    }
}