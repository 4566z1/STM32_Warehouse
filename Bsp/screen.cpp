#include "screen.h"
extern "C" {
#include "bsp_USART.h"
}

void Screen::init() { USART6_SendString("\x00\xff\xff\xff"); }

void Screen::set(const char* page, const char* var_name, const int& value)
{
    USART6_SendString("%s.%s.val=", page, var_name);
    USART6_SendString("%d", value);
    USART6_SendString("\xff\xff\xff");
}

void Screen::set(const char* page, const char* var_name, const char* value)
{
    USART6_SendString("%s.%s.txt=\"", page, var_name);
    USART6_SendString("%s", value);
    USART6_SendString("\"\xff\xff\xff");
}

bool Screen::has_data()
{
    uint8_t* rxdata = USART6_GetReceivedData();
    uint16_t rxNum = USART6_GetReceivedNum();

    if (rxNum) {
        memcpy(m_buf, rxdata, rxNum < 20 ? rxNum : 20);
        USART6_ClearReceived();
        return true;
    }

    return false;
}