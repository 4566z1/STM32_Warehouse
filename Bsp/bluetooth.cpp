#include "bluetooth.h"

extern "C" {
#include "bsp_USART.h"
}

bool BLE::decode()
{
    if (m_buf[0] != ':') return false;

    int i = 0;
    char* temp = strtok(m_buf, ":");

    while (temp) {
        if (i == 0) strcpy(m_ble_pack.name, temp);
        if (i == 1) strcpy(m_ble_pack.code, temp);
        if (i == 2) strcpy(m_ble_pack.mode, temp);
        temp = strtok(NULL, ":");

        ++i;
    }
    return true;
}

void BLE::read()
{
    // Refresh
    m_ble_pack = {{0}, {0}, {0}};
    memset(m_buf, 0, 20);

    // Read data
    uint8_t* rxdata = USART2_GetReceivedData();
    uint16_t rxNum = USART2_GetReceivedNum();
    memcpy(this->m_buf, rxdata, rxNum < 20 ? rxNum : 20);
    USART2_ClearReceived();
}

void BLE::send(const char* str) { USART2_SendString(str); }