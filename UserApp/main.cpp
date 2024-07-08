#include "common_inc.h"

extern "C"{
    #include "bsp_USART.h"
}
#include "utils.h"
#include "rfid.h"
#include "server.h"
#include "screen.h"

Rfid rfid;
Server server;
Screen screen;

enum Mode{
    MODE_ADD = 0,
    MODE_DEL = 1,
};

volatile Mode mode = MODE_DEL;

// RFID program entry
void rfid_main(void){
    vTaskDelay(1000);
    const char* prefix = "[rfid_main]";
    char card_epc[DATA_GROUP_LEN + 1] = {0};

    // Default mode is adding mode
    while(true){
        if(rfid.read(card_epc)){
            // Edit data in server
            char name[5] = {0}, code[5] = {0};
            sprintf(name, "%x", sign(card_epc, DATA_GROUP_LEN + 1));
            sprintf(code, "%d", card_epc[4]);

            if(mode == MODE_ADD)
                server.product_add(name, code);
            else
                server.product_del(name);

            // Show
            LOG("%s read: ", prefix);
            for(int i = 0; i < DATA_GROUP_LEN; ++i){
                LOG("%x ", card_epc[i]);
            }
            LOG("\n");
        } else { 
            LOG("%s read: %s", prefix, "The rfid-card is not found\n");
        }

        vTaskDelay(1000);
    }
}

// Screen program entry
void screen_main(void){
    const char* prefix = "[screen_main]";
    const int buf_size = 150; 
    char buf[buf_size] = {0};

    // Init the screen
    screen.send_command("\x00\xff\xff\xff");

    while(true){
        // Pull data from server
        if(server.product_get(buf, buf_size)){
            LOG("%s product_get: \n", prefix);
            LOG("%s\n", buf);

            screen.send_command("t0.txt=\"");
            screen.send_command(buf);
            screen.send_command("\"\xff\xff\xff");
        }

        //  Read data from screen
        screen.send_command("get bt0.val\xff\xff\xff");
        if(screen.get_command(buf, buf_size)){
            if(buf[0] == 0x71){
                mode = static_cast<Mode>(buf[1]);
            }
        } 

        vTaskDelay(1000);
    }
}