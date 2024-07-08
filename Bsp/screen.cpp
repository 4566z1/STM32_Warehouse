#include "screen.h"
extern "C"{
    #include "bsp_USART.h"
}

void Screen::send_command(const char* buf){
    USART6_SendString(buf);
}

bool Screen::get_command(char* buf, const int& size){
    uint8_t* rxdata = USART6_GetReceivedData();
    uint16_t rxNum = USART6_GetReceivedNum();
    if(rxdata[0] == '\0') {
        return false;
    }
    else {
        memcpy(buf, rxdata, rxNum > size ? size : rxNum);
        return true;
    }
}