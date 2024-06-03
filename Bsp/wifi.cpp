#include "wifi.h"
extern "C"{
    #include "bsp_USART.h"
}

#include <cstring>

void Wifi::set_server(char* host){
    USART3_SendString("S%s,", host);
}

void Wifi::product_add(char* name, char* cate){
    USART3_SendString("A%s,%s,", name, cate);
}

void Wifi::product_del(char* name){
    USART3_SendString("D%s,", name);
}