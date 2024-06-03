#include "common_inc.h"

extern "C"{
    #include "bsp_USART.h"
}
#include "rfid.h"
#include "wifi.h"

Rfid rfid;
Wifi wifi;

#include <cstdio>

void Main(void){
    HAL_Delay(2000);

    while(true){
        uint8_t card_epc[DATA_GROUP_LEN + 1] = {0};
        if(rfid.read(card_epc)){
            // Add to the server
            char name[2] = {0};
            char cate[2] = {0};

            name[0] = card_epc[0];
            cate[0] = card_epc[4];

            wifi.product_add(name, cate);

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