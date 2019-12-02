# DFRobot_IICSerial
This is an IIC to dual UART module with 1Mbps IIC transmission rate, and each sub UART is able to receive/transmit independent 256 bytes FIFO hardware cache. It can be used in the applications requiring the transmission of large amounts of data. <br>
The band rate, word length, and check format of every sub UART can be set independently. The module can provide at most 2Mbps communication rate, and support 4 IIC addresses. Four such modules can be connected to one controller board to expand 8 hardware serial port. <br>

![正反面svg效果图](https://github.com/Arya11111/DFRobot_MCP23017/blob/master/resources/images/SEN0245svg1.png)


## 产品链接（链接到英文商城）
    DFR0627 Gravity：IIC to dual UART module
   
## Table of Contents

* [Summary](#summary)
* [Installation](#installation)
* [Methods](#methods)
* [Compatibility](#compatibility)
* [History](#history)
* [Credits](#credits)

## Summary
1. Setup band rate<br>
2. Read serial data<br>
3. Write serial data<br>

## Installation

To use this library, first download the library file, paste it into the \Arduino\libraries directory, then open the examples folder and run the demo in the folder.

## Methods

```C++
/**
 * @brief Constructor
 * @param pWire I2C bus pointer object, default Wire
 * @param subUartChannel Sub UART channel, WK2132 has two sub UARTs: SUBUART_CHANNEL_1 or SUBUART_CHANNEL_2
 * @param IA1 corresponds with IA1 Level (0 or 1) of DIP switch on the module, and is used for configuring 
 * @n the IIC address of the 6th bit value(default: 1).
 * @param IA0 corresponds with IA0 Level (0 or 1) of DIP switch on the module, and is used for configuring
 * @n IIC address of the 5th bit value(default: 1).
 * @n The form of IIC address is shown below
 * 7   6   5   4   3   2   1   0
 * 0  IA1 IA0  1   0  C1  C0  0/1
 * @n IIC address only has 7 bits, while there are 8 bits for one byte, so the extra one bit will be filled as 0. 
 * @n The 6th bit corresponds with IA1 Level of DIP switch, can be configured manually. 
 * @n The 5th bit corresponds with IA0 Level of DIP switch, can be configured manually. 
 * @n The 4th and 3rd bits are fixed, value 1 and 0 respectively
 * @n The values of the 2nd and 1st bits are the sub UART channels, 00 for sub UART 1, 01 for sub UART 2. 
 * @n The 0 bit represents the operation object: 0 for register, 1 for FIFO cache.
 */
DFRobot_IICSerial(TwoWire &wire = Wire, uint8_t subUartChannel = SUBUART_CHANNEL_1, uint8_t IA1 = 1, uint8_t IA0 = 1);
~DFRobot_IICSerial();

/**
 * @brief Init function, set the band rate of sub UART
 * @param baud UART band rate
 */
void begin(long unsigned baud);

/**
 * @brief Init function, set the band rate of sub UART, data format. 
 * @param baud UART band rate 
 * @param format Sub UART data format: IICSerial_8N1, IICSerial_8N2, IICSerial_8Z1
 * @n IICSerial_8Z2, IICSerial_8O1, IICSerial_8O2, IICSerial_8E1, IICSerial_8E2
 * @n IICSerial_8F1, IICSerial_8F2
 */
void begin(long unsigned baud, uint8_t format);

/**
 * @brief Release sub UART to clean up all registers in Sub UART. Call function begin() again to make it work.
 */
void end();

/**
 * @brief Get the number of bytes in receive buffer, it should be the total number of bytes in FIFO
 * @n receive buffer(256B) and self-defined _rx_buffer(31B).
 * @return Return the number of bytes in receive buffer 
 */
int available(void);

/**
 * @brief Return the data of 1 byte without deleting the data in the receive buffer
 * @return Return the readings
 */
int peek(void);

/**
 * @brief Read 1 byte in receive buffer, this operation will delete the data in the buffer.
 * @return Return the readings
 */
int read(void);

/**
 * @brief Read a specified number of character from FIFO and store them into a array. 
 * @n This operation doesn't involve receive buffer. 
 * @param pBuf Array for storing data 
 * @param size The number of character to be read
 * @return Return the readings
 */
size_t read(void *pBuf, size_t size);

/**
 * @brief Wait for the data to be transmited completely 
 */
void flush(void);

/**
 * @brief Write one byte into transmit FIFO cache.
 * @n The following are the overload functions of the byte of different data type. 
 * @return Return 0 if it succeeds, otherwise return -1
 */
virtual size_t write(uint8_t);
inline size_t write(unsigned long n);
inline size_t write(long n);
inline size_t write(unsigned int n);
inline size_t write(int n);

/**
 * @brief Write data into transmit FIFO cache 
 * @param pBuf Store buffer for the data to be read
 * @param size Length of the data to be read 
 * @return Output the number of bytes
 */
size_t write(const uint8_t *pBuf, size_t size);
```

## Compatibility

MCU                | Work Well    | Work Wrong   | Untested    | Remarks
------------------ | :----------: | :----------: | :---------: | -----
Arduino Uno        |      √       |              |             | 
Mega2560        |      √       |              |             | 
Leonardo        |      √       |              |             | 
ESP32         |      √       |              |             | 
micro:bit        |      √       |              |             | 

## History

- Date 2019-8-7
- Version V1.0

## Credits

Written by(xue.peng@dfrobot.com), 2019. (Welcome to our [website](https://www.dfrobot.com/))





