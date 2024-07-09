#ifndef BLUETOOTH_H
#define BLUETOOTH_H

struct BLE_PACK {
    char name[5];
    char code[5];
    char mode[5];
};

class BLE
{
   public:
    explicit BLE() {}

    void send(const char* data);
    void read(char* data, const int& len);
    bool decode(char* buf, BLE_PACK* ble_pack);

   private:
};

#endif