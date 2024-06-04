#include "common_inc.h"

extern "C"{
    #include "bsp_USART.h"
}
#include "utils.h"
#include "rfid.h"
#include "wifi.h"
#include "screen.h"

Rfid rfid;
Wifi wifi;
Screen screen;

enum MODE{
    MODE_ADD = 0,
    MODE_DEL = 1,
};

// RFID program entry
MODE mode = MODE_ADD;

void RFIDMain(void){
    const char* prefix = "[RFIDMAIN]";
    vTaskDelay(2000);

    // Default mode is adding mode
    while(true){
        char card_epc[DATA_GROUP_LEN + 1] = {0};
        if(rfid.read(card_epc)){
            // Edit data in server
            char name[5] = {0}, code[5] = {0};
            sprintf(name, "%x", sign(card_epc, DATA_GROUP_LEN + 1));
            sprintf(code, "%d", card_epc[4]);

            if(mode == MODE_ADD)
                wifi.product_add(name, code);
            else
                wifi.product_del(name);

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
void SCREENMain(void){
    const char* prefix = "[SCREENMAIN]";
    const int buf_size = 80;
    char buf[buf_size] = {0};

    while(true){
        // Pull data from server
        if(wifi.product_get(buf, buf_size)){
            LOG("%s product_get: \n", prefix);
            for(int i = 0; i < buf_size; ++i){
                if(buf[i] == 0) break;
                if(buf[i] == '@'){ buf[i] = '\n'; }
            }
            LOG("%s", buf);

            screen.set_text(buf);
        }

        //  Read data from screen
        screen.send_command("get r0.val");
        if(screen.get_command(buf, buf_size)){
            if(buf[0] == 0x71){
                mode = static_cast<MODE>(buf[1]);
            }
        } 

        vTaskDelay(1000);
    }
}