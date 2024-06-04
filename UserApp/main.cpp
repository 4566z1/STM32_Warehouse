#include "common_inc.h"

extern "C"{
    #include "bsp_USART.h"
}
#include "utils.h"
#include "rfid.h"
#include "wifi.h"

Rfid rfid;
Wifi wifi;

void Main(void){
    HAL_Delay(2000);

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
            for(int i = 0; i < DATA_GROUP_LEN; ++i){
                USART1_SendString("%x ", card_epc[i]);
            }
            USART1_SendString("\n");
        } else { 
            USART1_SendString("The rfid-card is not found\n");
        }

        HAL_Delay(100);
    }
}