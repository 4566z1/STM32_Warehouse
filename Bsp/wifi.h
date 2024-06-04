#ifndef WIFI_H
#define WIFI_H

#include "stm32f4xx.h"

class Wifi{
    public:
        explicit Wifi() 
        {}

        void set_server(const char* host);
        void product_add(const char* name, const char* cate);
        void product_del(const char* name);
    private:
        
};

#endif