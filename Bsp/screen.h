#ifndef SCREEN_H
#define SCREEN_H

#include "stm32f4xx.h"

class Screen
{
   public:
    explicit Screen() {}

    void init();

    const char* get_data();
    void get(const char* page, const char* var_name);
    void set(const char* page, const char* var_name, const int& value);
    void set(const char* page, const char* var_name, const char* value);

   private:
    char m_buf[20] = {0};
};

#endif