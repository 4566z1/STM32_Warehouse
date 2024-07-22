#include "server.h"

extern "C" {
#include "bsp_USART.h"
#include "cmsis_os.h"
}

#include <cstring>

void Server::set_server(const char* host) { USART3_SendString("S%s,", host); }

bool Server::has_data()
{
    uint8_t* rxdata = USART3_GetReceivedData();
    uint16_t rxNum = USART3_GetReceivedNum();

    // Double-buffer-swap
    if (rxNum) {
        // Fillter the \r \n
        char* p = (char*)rxdata;
        while (*p == '\r' || *p == '\n') ++p;

        if (strcmp(this->m_swap_buf, p)) {
            strcpy(this->m_swap_buf, p);
            USART3_ClearReceived();
            return true;
        }
    }

    USART3_ClearReceived();
    return false;
}

void Server::product_add(const char* name, const char* cate) { USART3_SendString("A%s,%s,", name, cate); }

void Server::product_del(const char* name) { USART3_SendString("D%s,", name); }