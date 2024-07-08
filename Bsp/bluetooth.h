#ifndef BLUETOOTH_H
#define BLUETOOTH_H

struct _BLE_PACK{
   char name[5];
   char code[5];
   char mode[5];
};

class BLE
{
   public:
    explicit BLE() 
    {}

    void send(const char* data);
    void read(char* data, const int& len);

   private:
};

using BLE_PACK = struct _BLE_PACK;

extern void decode(char* buf, BLE_PACK* ble_pack);

#endif