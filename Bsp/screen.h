#ifndef SCREEN_H
#define SCREEN_H

#include "stm32f4xx.h"

class Screen
{
   public:
    explicit Screen() {}

    void init() { this->send_command("\x00\xff\xff\xff"); }
    void send_command(const char* buf);
    bool get_command(char* buf, const int& size);

   private:
};

#endif