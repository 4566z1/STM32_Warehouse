#include "bluetooth.h"

extern "C" {
#include "bsp_USART.h"
}

void decode(char* buf, BLE_PACK* ble_pack)
{
    int i = 0;
    char *temp = strtok(buf, ":");

    while(temp)
    {
        if(i == 0) strcpy(ble_pack->name, temp);
        if(i == 1) strcpy(ble_pack->code, temp);
        if(i == 2) strcpy(ble_pack->mode, temp);
        temp = strtok(NULL, ":");

        ++i;
    }
}

void BLE::read(char* data, const int& len)
{
    uint8_t* rxdata = USART2_GetReceivedData();
    uint16_t rxNum = USART2_GetReceivedNum();
    memcpy(data, rxdata, rxNum < len ? rxNum : len);
    USART2_ClearReceived();
}

void BLE::send(const char* str) { USART2_SendString(str); }