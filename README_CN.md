DFRobot_IICSerial
===========================

* [English Version](./README.md)

这是一款IIC转双串口的UART模块，IIC最高速率为1Mbps。每个子串口具备收/发独立的256字节FIFO硬件缓存，可以满足用户大量数据传输的场景。<br>
每个子串口的波特率，字长，校验格式可独立设置，最高提供2M bps的通信速率，支持4个IIC地址，一块主控上最多并联4个模块，一次最多扩展8个硬串口。<br>

![产品效果图](./resources/images/DFR0627.png)


## Product Link（[https://www.dfrobot.com.cn/goods-2668.html](https://www.dfrobot.com.cn/goods-2668.html)）
    SKU: DFR0627   
   
## Table of Contents

* [概述](#概述)
* [库安装](#库安装)
* [方法](#方法)
* [兼容性](#兼容性)
* [历史](#历史)
* [创作者](#创作者)

## 概述
1. 波特率设置；<br>
2. 串口数据读；<br>
3. 串口数据写；<br>

## 库安装

使用此库前，请首先下载库文件，将其粘贴到\Arduino\libraries目录中，然后打开examples文件夹并在该文件夹中运行演示。

## 方法

```C++
  /**
   * @fn DFRobot_IIC_Serial
   * @brief 构造函数
   * @param pWire I2C总线指针对象，构造设备，可传参数也可不传参数，默认Wire
   * @param subUartChannel 子串口通道号，WK2132有2个子串口，可填SUBUART_CHANNEL_1或SUBUART_CHANNEL_2
   * @param IA1 对应模块上的拨码开关IA1的电平（0或1），用于配置IIC地址第6位的值，默认为1
   * @param IA0 对应模块上的拨码开关IA0的电平（0或1），用于配置IIC地址第5位的值，默认为1
   * @n 模块IIC地址由构成如下图所示
   * @n 7   6   5   4   3   2   1   0
   * @n 0  IA1 IA0  1   0  C1  C0  0/1
   * @n IIC地址是7位地址，而一个字节有8位，多出的一位一般用0处理
   * @n 第6位值对应拨码开关的IA1，可手动配置
   * @n 第5位值对应拨码开关的IA0，可手动配置
   * @n 第4/3位为固定值，分别为1/0
   * @n 第2/1位的值为子串口通道，00表示子串口1，01表示子串口2
   * @n 第0位表示操作对象是寄存器还是FIFO缓存，0表示寄存器，1表示FIFO缓存
   */
  DFRobot_IIC_Serial(TwoWire &wire = Wire, uint8_t subUartChannel = SUBUART_CHANNEL_1, uint8_t IA1 = 1, uint8_t IA0 = 1);
  ~DFRobot_IIC_Serial();
  
  /**
   * @fn begin
   * @brief 初始化函数，设置子串口的波特率
   * @param baud 串口波特率
   */
  void begin(long unsigned baud);
  
  /**
   * @fn begin
   * @brief 初始化函数，设置子串口的波特率，数据格式
   * @param baud 串口波特率
   * @param format 子串口数据格式，可填IIC_SERIAL_8N1、IIC_SERIAL_8N2、IIC_SERIAL_8Z1
   * @n IIC_SERIAL_8Z2、IIC_SERIAL_8O1、IIC_SERIAL_8O2、IIC_SERIAL_8E1、IIC_SERIAL_8E2
   * @n IIC_SERIAL_8F1、IIC_SERIAL_8F2等参数
   */
  void begin(long unsigned baud, uint8_t format);
  
  /**
   * @fn end
   * @brief 释放子串口，该操作后，所有子串口寄存器被清空,需再次begin(),才可正常工作
   */
  void end();
  
  /**
   * @fn available
   * @brief 获取接收缓冲区的字节数，该字节数是FIFO接收缓存（256B）和自定义_rx_buffer（31B）中总的字节数
   * @return 返回接收缓存中的字节个数
   */
  int available(void);
  
  /**
   * @fn peek
   * @brief 返回1字节的数据，但不会从接收缓存中删除该数据
   * @return 返回读取的数据
   */
  int peek(void);
  
  /**
   * @fn read
   * @brief 从接收缓存中读取一个字节，该读取会清除缓存中的数据
   * @return 返回读取的数据
   */
  int read(void);
  
  /**
   * @fn read
   * @brief 从接收FIFO中读取指定长度的字符，并将其存入一个数组中,该读取不会经过接收缓存。
   * @param pBuf 用于存储数据的数组
   * @param size 要读取的字符的长度
   * @return 返回实际读取的字节数
   */
  size_t read(void *pBuf, size_t size);
  
  /**
   * @fn flush
   * @brief 等待正在发送的数据发送完成
   */
  void flush(void);
  
  /**
   * @fn write
   * @brief 向发送FIFO缓存中写数据,以下为不同数据类型字节的重载函数 
   * @return 成功返回0，否者返回-1
   */
  virtual size_t write(uint8_t n);
  inline size_t write(unsigned long n);
  inline size_t write(long n);
  inline size_t write(unsigned int n);
  inline size_t write(int n);
  
  /**
   * @fn write
   * @brief 向发送FIFO缓存中写入数据
   * @param pBuf 要读取数据的存放缓存
   * @param size 要读取数据的长度
   * @return 输出的字节数
   */
  size_t write(const uint8_t *pBuf, size_t size);
```

## 兼容性

MCU                | Work Well    | Work Wrong   | Untested    | Remarks
------------------ | :----------: | :----------: | :---------: | -----
Arduino Uno        |      √       |              |             | 
Mega2560        |      √       |              |             | 
Leonardo        |      √       |              |             | 
ESP32         |      √       |              |             | 
micro:bit        |      √       |              |             | 

## 历史

- 2019/08/07 - 1.0.0 版本.

## 创作者

Written by Arya(xue.peng@dfrobot.com), 2021. (Welcome to our [website](https://www.dfrobot.com/))




