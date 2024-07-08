#include "common_inc.h"

extern "C" {
#include "bsp_USART.h"
}
#include "aht10.h"
#include "bluetooth.h"
#include "screen.h"
#include "server.h"

AHT10 aht10(0x70);
BLE ble;
Server server;
Screen screen;

// RFID program entry
void rfid_main(void)
{
    vTaskDelay(1000);
    const char* prefix = "rfid_main";

    while (true) {
        BLE_PACK ble_pack = {0};
        char buf[20] = {0};
        ble.read(buf, 20);

        if (buf[0] == ':') {
            decode(buf, &ble_pack);

            LOG("%s => name: %s code: %s mode: %s", prefix, ble_pack.name, ble_pack.code, ble_pack.mode);

            int mode = atoi(ble_pack.mode);
            if (mode == 0)
                server.product_add(ble_pack.name, ble_pack.code);
            else
                server.product_del(ble_pack.name);

        } else {
            LOG("%s => %s", prefix, "Bluetooth is no data.\n");
        }

        vTaskDelay(100);
    }
}

// Screen program entry
void screen_main(void)
{
    vTaskDelay(1000);
    const char* prefix = "screen_main";
    const int buf_size = 100;
    char swap_buf[buf_size] = {0};
    float temperature, humidity;

    // Init the screen
    screen.send_command("\x00\xff\xff\xff");

    while (true) {
        // Temperature && humidity
        {
            aht10.read(temperature, humidity);

            char temp_str[10] = {0}, humid_str[10] = {0};
            sprintf(temp_str, "%d", (int)temperature);
            sprintf(humid_str, "%d%%", (int)humidity);
            
            LOG("%s temp_str: %s\n", prefix, temp_str);
            LOG("%s temp_str: %s\n", prefix, humid_str);

            screen.send_command("sensor.t2.txt=\"");
            screen.send_command(temp_str);
            screen.send_command("\"\xff\xff\xff");

            screen.send_command("sensor.t3.txt=\"");
            screen.send_command(humid_str);
            screen.send_command("\"\xff\xff\xff");
        }

        // Warehouse data
        {
            char buf[buf_size] = {0};

            if (server.product_get(buf, buf_size)) {
                // Update data if different
                if (strcmp(swap_buf, buf)) {
                    LOG("%s product_get: \n", prefix);
                    LOG("%s\n", buf);

                    screen.send_command("ware.t0.txt=\"");
                    screen.send_command(buf);
                    screen.send_command("\"\xff\xff\xff");

                    strcpy(swap_buf, buf);
                }
            }
        }

        // Controller
        {
            char buf_tem[20] = {0}, buf_light[20] = {0};

            screen.send_command("get sensor.n0.val\xff\xff\xff");
            screen.get_command(buf_tem, 20);
            if (buf_tem[0] == 0x71) {
                if (temperature > buf_tem[1]) {
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