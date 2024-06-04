#include "common_inc.h"

extern "C"{
    #include "bsp_USART.h"
}
#include "utils.h"
#include "rfid.h"
#include "wifi.h"


Rfid rfid;
Wifi wifi;

// RFID program entry
void RFIDMain(void){
    const char* prefix = "[RFIDMAIN]";
    vTaskDelay(2000);

    // Default mode is adding mode.
    int mode = 0;   
    while(true){
        char card_epc[DATA_GROUP_LEN + 1] = {0};
        if(rfid.read(card_epc)){
            // Add data to the server
            char name[5] = {0}, code[5] = {0};
            sprintf(name, "%x", sign(card_epc, DATA_GROUP_LEN + 1));
            sprintf(code, "%d", card_epc[4]);

            wifi.product_add(name, code);

            // Show
            LOG("%s ", prefix);
            for(int i = 0; i < DATA_GROUP_LEN; ++i){
                LOG("%x ", card_epc[i]);
            }
            LOG("\n");
        } else { 
            LOG("%s %s", prefix, "The rfid-card is not found\n");
        }

        vTaskDelay(1000);
    }
}

// Screen program entry
void SCREENMain(void){
    const char* prefix = "[SCREENMAIN]";
    const int buf_size = 50;
    char buf[buf_size] = {0};

    while(true){
        if(wifi.product_get(buf, buf_size)){
            LOG("%s\n", prefix);
            for(int i = 0; i < buf_size; ++i){
                if(buf[i] == 0) break;
                if(buf[i] == '@'){ buf[i] = '\n'; }
            }
            LOG("%s", buf);
        }
        vTaskDelay(1000);
    }
}