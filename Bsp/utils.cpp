#include "utils.h"

int sign(const char* buf, const int& size){
    int res = 0;
    for(int i = 0; i < size; ++i){
        res += buf[i] ^ 0xE5;
    }
    return res;
}