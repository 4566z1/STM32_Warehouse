#ifndef SERVER_H
#define SERVER_H

#include "stm32f4xx.h"

class Server
{
   public:
    explicit Server() {}

    const char* get_data() { return m_swap_buf; }
    bool has_data();

    void set_server(const char* host);
    void product_add(const char* name, const char* cate);
    void product_del(const char* name);

   private:
    char m_swap_buf[512] = {0};
};

#endif