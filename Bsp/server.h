#ifndef SERVER_H
#define SERVER_H

#include "stm32f4xx.h"

class Server
{
   public:
    explicit Server() {}

    const char* get_product() { return m_buf; }
    
    void set_server(const char* host);
    bool product_get();
    void product_add(const char* name, const char* cate);
    void product_del(const char* name);

   private:
    // 双缓冲
    char m_buf[512] = {0};
    char m_swap_buf[512] = {0};
};

#endif