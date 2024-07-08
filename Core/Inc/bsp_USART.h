#ifndef __BSP__USART_H
#define __BSP__USART_H
/***********************************************************************************************************************************
 ** 【代码编写】  魔女开发板团队
 ** 【淘    宝】  魔女开发板       https://demoboard.taobao.com
 ***********************************************************************************************************************************
 ** 【文件名称】  bsp_usart.h
 **
 ** 【 功  能 】  串口通信(USART1、USART2、USART3、UART4、UART5、USART6)
 **               波特率-None-8-1
 **               调用全局声明中的各串口函数，即可完成初始化、发送、接收
 **
 ** 【适用平台】  STM32F407 + keil5 + HAL库 / 标准库
 **
 ** 【 CubeMX 】  可以、但无需，通过CubeMX、CubeIDE使能、打勾串口(方便标记使用哪些引脚）; 无论有没有通过CubeMX使能、打勾，都能使用本文件功能。
 **               如果通过CubeMX使能了串口，还可以使用标准HAL库的部分函数。但不建议。
 **               重点、重点、重点：
 **               不能在CubeMX、CubeIDE中配置串口的"全局中断"、"DMA", 否则冲突，无法使用本文件功能。
 **
 ** 【移植说明】  1- 复制本工程的USART文件夹，到目标工程文件夹中;
 **               2- 添加文件存放路径 (Option > C/C++ > Include Paths );
 **               3- 添加文件到工程 (在Keil左侧工程管理器中双击目标文件夹);
 **               4- 添加文件引用 #include "bsp_usart.h＂(哪个文件要用串口功能，就在文件顶部添加);
 **
 ** 【串口引脚】  调用各USARTx_Init()函数时，将按以下默认引脚进行初始化：
 **               1- USART1  TX_PA9   RX_PA10       特别地说明：魔女开发板，已板载虚拟串口，连接了USART1, 使用和烧录用的同一USB接口，即可通过USART1和电脑进行通信。具体查看资料文件夹中的说明文件。
 **               2- USART2  TX_PA2   RX_PA3
 **               3- USART3  TX_PB10  RX_PB11
 **               4- UART4   TX_PC10  RX_PC11
 **               5- UART5   TX_PC12  RX_PD2
 **               6- USART6  TX_PC6   RX_PC7
 **
 ** 【代码使用】  每组串口，已封装6个函数，以串口1作如下说明：
 **               初始化:             void      USART1_Init(uint32_t baudrate);                  // 初始化串口; 配置GPIO引脚PA9+PA10、配置通信参数：波特率-None-8-1、配置中断
 **               发送字符串:         void      USART1_SendString(const char *string,...);       // 发送字符串;   参数：字符串地址; 使用方法如同printf
 **               发送指定长度：      void      USART1_SendData(uint8_t *data, uint16_t num);    // 发送指定数据; 参数：数据地址、字节数
 **               获取接收的字节数:   uint16_t  USART1_GetReceivedNum(void);                     // 获取接收到的字节数; 如果返回值大于0，即为接收到新一帧数据
 **               获取接收的数据：    uint8_t*  USART1_GetReceivedData(void);                    // 获取接收到的数据(缓存的地址)
 **               清零接收标志：      void      USART1_ClearReceived(void);                      // 清0接收标志，即清0接收到的字节数; 每次处理完成数据，就要调用这个函数清0
 **
 **
 ** 【主要更新】  2024-01-09  取消部分静态变量，并入结构体中，使用调用更直观
 **               2024-01-03  完善函数、注释
 **               2023-12-25  增加接收函数的封装，取消接收变量(全局)的使用
 **               2023-12-23  完成对HAL库的支持
 **               2023-01-27  增加宏定义、完善注释
 **               2021-12-16  完善接收机制：取消接收标志，判断接收字节数>0即为接收到新数据
 **               2021-11-03  完善接收函数返回值处理
 **               2021-08-14  增加宏定义：接收缓存区大小设定值，使空间占用更可控；
 **               2021-08-14  修改发送函数名称：USARTx_Printf()，修改为：USARTx_SendString();
 **               2020-09-02  文件建立、USART1接收中断、空闲中断、发送中断、DMA收发
 **               2021-06-04  USART1、2、3及UART4、5 的收发完善代码
 **               2021-06-09  完善文件格式、注释
 **               2021-06-22  完善注释说明
 **
************************************************************************************************************************************/
#include "stm32f4xx_hal.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"




/*****************************************************************************
 ** 移植配置修改区
 ** 备注：除非有特殊要求，否则，下面参数已通用如蓝牙通信、ESP8266通信、串口屏等
****************************************************************************/
// 串口开关
#define USART1_EN                       1          // 串口1，0=关、1=启用;  倘若没用到USART1, 置0，就不会开辟USART1发送缓存、接收缓存，省一点资源;
#define USART2_EN                       1          // 串口2，0=关、1=启用;  同上;
#define USART3_EN                       1          // 串口3，0=关、1=启用;  同上;
#define UART4_EN                        1          // 串口4，0=关、1=启用;  同上;
#define UART5_EN                        1          // 串口5，0=关、1=启用;  同上;
#define USART6_EN                       1          // 串口5，0=关、1=启用;  同上;
// 发送缓冲区大小
#define USART1_TX_BUF_SIZE            512          // 配置每组USART发送环形缓冲区数组的大小，单位：字节数; 
#define USART2_TX_BUF_SIZE            512          // -- 只有在前面串口开关在打开状态，才会定义具体的缓冲区数组
#define USART3_TX_BUF_SIZE            512          // -- 默认值：512，此值已能适配大部场景的通信; 如果与ESP8266之类的设备通信，可适当增大此值。
#define UART4_TX_BUF_SIZE             512          // -- 值范围：1~65535; 注意初始化后，不要超过芯片最大RAM值。
#define UART5_TX_BUF_SIZE             512          // -- 注意此值是一个环形缓冲区大小，决定每一帧或多帧数据进入发送前的总缓存字节数，先进先出。
#define USART6_TX_BUF_SIZE            512          //
// 接收缓冲区大小
#define USART1_RX_BUF_SIZE            512          // 配置每组USART接收缓冲区的大小，单位：字节; 此值影响范围：中断里的接收缓存大小，接收后数据缓存的大小
#define USART2_RX_BUF_SIZE            512          // --- 当接收到的一帧数据字节数，小于此值时，数据正常；
#define USART3_RX_BUF_SIZE            512          // --- 当接收到的一帧数据字节数，超过此值时，超出部分，将在中断中直接弃舍，直到此帧接收结束(发生空闲中断); 
#define UART4_RX_BUF_SIZE             512          // 
#define UART5_RX_BUF_SIZE             512          // 
#define USART6_RX_BUF_SIZE            512          // 
// 结束-配置修改

#define LOG USART1_SendString

/*****************************************************************************
 ** 声明全局函数
 **
 ** 每组串口，统一：
 ** 1个初始化函数 (波特率-None-8-1)
 ** 2个发送函数   (发送字符串、指定字节数数据)
 ** 2个接收函数   (获取接收字节数、获取数据地址）
 ** 1个清0函数    (清0接收字节数，才能进行下轮判断)
****************************************************************************/
// USART1
void     USART1_Init(uint32_t baudrate);                    // 初始化串口1; GPIO引脚PA9+PA10、中断优先级、通信参数：波特率可设、8位数据、无校验、1个停止位
void     USART1_SendString(const char *string, ...);        // 发送字符串;   参数：字符串地址; 使用方法如同printf
void     USART1_SendData(uint8_t *buffer, uint16_t num);    // 发送指定数据; 参数：数据地址、字节数
uint16_t USART1_GetReceivedNum(void);                       // 获取接收到的最新一帧字节数
uint8_t *USART1_GetReceivedData(void);                      // 获取接收到的数据(缓存的地址)
void     USART1_ClearReceived(void);                        // 清理接收到的数据(清理最后一帧字节数，因为它是判断接收的标志)
// USART2
void     USART2_Init(uint32_t baudrate);                    // 初始化串口2; GPIO引脚PA2+PA3、中断优先级、通信参数：波特率可设、8位数据、无校验、1个停止位
void     USART2_SendString(const char *string, ...);        // 发送字符串;   参数：字符串地址; 使用方法如同printf
void     USART2_SendData(uint8_t *buffer, uint16_t num);    // 发送指定数据; 参数：数据地址、字节数
uint16_t USART2_GetReceivedNum(void);                       // 获取接收到的最新一帧字节数
uint8_t *USART2_GetReceivedData(void);                      // 获取接收到的数据(缓存的地址)
void     USART2_ClearReceived(void);                        // 清理接收到的数据(清理最后一帧字节数，因为它是判断接收的标志)
// USART3
void     USART3_Init(uint32_t baudrate);                    // 初始化串口3; GPIO引脚PB10+PB11、中断优先级、通信参数：波特率可设、8位数据、无校验、1个停止位
void     USART3_SendString(const char *string, ...);        // 发送字符串;   参数：字符串地址; 使用方法如同printf
void     USART3_SendData(uint8_t *buffer, uint16_t num);    // 发送指定数据; 参数：数据地址、字节数
uint16_t USART3_GetReceivedNum(void);                       // 获取接收到的最新一帧字节数
uint8_t *USART3_GetReceivedData(void);                      // 获取接收到的数据(缓存的地址)
void     USART3_ClearReceived(void);                        // 清理接收到的数据(清理最后一帧字节数，因为它是判断接收的标志)
// UART4
void     UART4_Init(uint32_t baudrate);                     // 初始化串口4; GPIO引脚PC10+PC11、中断优先级、通信参数：波特率可设、8位数据、无校验、1个停止位
void     UART4_SendString(const char *string, ...);         // 发送字符串;   参数：字符串地址; 使用方法如同printf
void     UART4_SendData(uint8_t *buffer, uint16_t num);     // 发送指定数据; 参数：数据地址、字节数
uint16_t UART4_GetReceivedNum(void);                        // 获取接收到的最新一帧字节数
uint8_t *UART4_GetReceivedData(void);                       // 获取接收到的数据(缓存的地址)
void     UART4_ClearReceived(void);                         // 清理接收到的数据(清理最后一帧字节数，因为它是判断接收的标志)
// UART5
void     UART5_Init(uint32_t baudrate);                     // 初始化串口5; GPIO引脚PC12+PD2、中断优先级、通信参数：波特率可设、8位数据、无校验、1个停止位
void     UART5_SendString(const char *string, ...);         // 发送字符串;   参数：字符串地址; 使用方法如同printf
void     UART5_SendData(uint8_t *buffer, uint16_t num);     // 发送指定数据; 参数：数据地址、字节数
uint16_t UART5_GetReceivedNum(void);                        // 获取接收到的最新一帧字节数
uint8_t *UART5_GetReceivedData(void);                       // 获取接收到的数据(缓存的地址)
void     UART5_ClearReceived(void);                         // 清理接收到的数据(清理最后一帧字节数，因为它是判断接收的标志)
// UART5
void     USART6_Init(uint32_t baudrate);                    // 初始化串口6; GPIO引脚PC6+PC7、中断优先级、通信参数：波特率可设、8位数据、无校验、1个停止位
void     USART6_SendString(const char *string, ...);        // 发送字符串;   参数：字符串地址; 使用方法如同printf
void     USART6_SendData(uint8_t *buffer, uint16_t num);    // 发送指定数据; 参数：数据地址、字节数
uint16_t USART6_GetReceivedNum(void);                       // 获取接收到的最新一帧字节数
uint8_t *USART6_GetReceivedData(void);                      // 获取接收到的数据(缓存的地址)
void     USART6_ClearReceived(void);                        // 清理接收到的数据(清理最后一帧字节数，因为它是判断接收的标志)
// 辅助功能
void     showData(uint8_t *rxData, uint16_t rxNum);         // 把数据，经串口1，发送到串口助手上，方便观察
// 结束-全局函数声明

#endif

