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

void Screen::get(const char* page, const char* var_name)
{
    USART6_SendString("get %s.%s.val\xff\xff\xff", page, var_name);
}

const char* Screen::get_data()
{
    uint8_t* rxdata = USART6_GetReceivedData();
    uint16_t rxNum = USART6_GetReceivedNum();
    if (rxdata[0] != '\0') {
        memcpy(m_buf, rxdata, rxNum < 20 ? rxNum : 20);
    }
    USART6_ClearReceived();
    return m_buf;
}