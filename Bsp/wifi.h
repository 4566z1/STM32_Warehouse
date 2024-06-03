#ifndef WIFI_H
#define WIFI_H

#include "stm32f4xx.h"

class Wifi{
    public:
        explicit Wifi() 
        {}

        void set_server(char* host);
        void product_add(char* name, char* cate);
        void product_del(char* name);
    private:
        
};

#endif