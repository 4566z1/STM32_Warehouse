#ifndef BLUETOOTH_H
#define BLUETOOTH_H

class BLE
{
   public:
    explicit BLE() {}

    struct BLE_PACK {
        char name[5];
        char code[5];
        char mode[5];
    };

    BLE_PACK* get() { return &m_ble_pack; }
    void send(const char* data);
    bool read();

   private:
    bool decode(char* data);

    BLE_PACK m_ble_pack = {{0}, {0}, {0}};
};

#endif