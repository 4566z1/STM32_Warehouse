#include "wifi.h"
#include "usart.h"

extern "C"{
    #include "cmsis_os.h"
    #include "bsp_USART.h"
}

#include <cstring>

void Wifi::set_server(const char* host){
    USART3_SendString("S%s,", host);
}

bool Wifi::product_get(char* buf, const int& size){
    USART3_SendString("G,");

    vTaskDelay(1000);
    
    uint8_t* rxdata = USART3_GetReceivedData();
    uint16_t rxNum = USART3_GetReceivedNum();
    if(rxdata == 0) {
        return false;
    }
    else {
        // Fillter the /r/n
        memcpy(buf, rxdata + 2, rxNum > size ? size : rxNum);
        return true;
    }
}

void Wifi::product_add(const char* name, const char* cate){
    USART3_SendString("A%s,%s,", name, cate);
}

void Wifi::product_del(const char* name){
    USART3_SendString("D%s,", name);
}