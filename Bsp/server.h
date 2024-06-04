#ifndef SERVER_H
#define SERVER_H

#include "stm32f4xx.h"

class Server
{
    public:
        explicit Server() 
        {}

        void set_server(const char* host);
        bool product_get(char* buf, const int& size);
        void product_add(const char* name, const char* cate);
        void product_del(const char* name);
    private:
        
};

#endif