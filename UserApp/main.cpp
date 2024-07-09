#include "aht10.h"
#include "bluetooth.h"
#include "common_inc.h"
#include "screen.h"
#include "server.h"

extern "C" {
#include "bsp_USART.h"
}

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
            LOG("rfid_main => name: %s code: %s mode: %s", ble.get()->name, ble.get()->code, ble.get()->mode);

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

            LOG("screen_main => temp_str: %s\n", aht10.get_tem());
            LOG("screen_main => temp_str: %s\n", aht10.get_humi());

            screen.send_command("sensor.t2.txt=\"");
            screen.send_command(aht10.get_tem_str());
            screen.send_command("\"\xff\xff\xff");

            screen.send_command("sensor.t3.txt=\"");
            screen.send_command(aht10.get_humi_str());
            screen.send_command("\"\xff\xff\xff");
        }

        // Warehouse data
        {
            if (server.product_get()) {
                screen.send_command("ware.t0.txt=\"");
                screen.send_command(server.get_product());
                screen.send_command("\"\xff\xff\xff");
            }
        }

        // Controller
        {
            char buf_tem[20] = {0}, buf_light[20] = {0};

            screen.send_command("get sensor.n0.val\xff\xff\xff");
            screen.get_command(buf_tem, 20);
            if (buf_tem[0] == 0x71) {
                if ((int)aht10.get_tem() > buf_tem[1]) {
                    // Open
                }
            }

            screen.send_command("get sensor.bt1.val\xff\xff\xff");
            screen.get_command(buf_light, 20);
            if (buf_light[0] == 0x71) {
                if (buf_light[1] == 0x01) {
                    // Open Light
                } else {
                    // Close Light
                }
            }
        }

        vTaskDelay(100);
    }
}