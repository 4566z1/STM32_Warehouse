#include "bluetooth.h"

extern "C" {
#include "bsp_USART.h"
}

bool BLE::decode(char* data)
{
    if (data[0] != ':') return false;

    int i = 0;
    char* temp = strtok(data, ":");

    while (temp) {
        if (i == 0) strcpy(m_ble_pack.name, temp);
        if (i == 1) strcpy(m_ble_pack.code, temp);
        if (i == 2) strcpy(m_ble_pack.mode, temp);
        temp = strtok(NULL, ":");

        ++i;
    }
    return true;
}

bool BLE::read()
{
    uint8_t* rxdata = USART2_GetReceivedData();

    /*    数据有效    */
    if (rxdata[0] != '\0') {
        m_ble_pack = {{0}, {0}, {0}};
        bool ret = decode((char*)rxdata);
        USART2_ClearReceived();
        return ret;
    }

    return false;
}

void BLE::send(const char* str) { USART2_SendString(str); }