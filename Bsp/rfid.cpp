#include "rfid.h"
#include "usart.h"

extern "C"{
    #include "bsp_USART.h"
}

struct RFID_READ_HEADER{
    char Header[2];
    char FrameType;
    char Address[2];
    char FrameCode;
    char ParamLength[2];
};

bool Rfid::read(uint8_t* data, uint16_t size){
    RFID_READ_HEADER header = {0};
    char* command = const_cast<char*>("\x52\x46\x00\x00\x00\x22\x00\x00\x46");
    //uint8_t buf[USART2_RX_BUF_SIZE] = {0};

    // 读取 Header
    USART2_SendData(reinterpret_cast<uint8_t*>(command), 9);
    HAL_Delay(1000);
    uint8_t* rxdata = USART2_GetReceivedData();
    uint16_t rxNum = USART2_GetReceivedNum();
    memcpy(&header, rxdata, sizeof(header));
    USART2_ClearReceived();

    if(header.ParamLength[1] / DATA_GROUP_LEN <= 0) 
        return false;
    
    // 读取
    for (int i = 0; i < DATA_GROUP_LEN - 1; ++i) {
        data[i] = *(rxdata + sizeof(header) + i);
    }
    return true;
}