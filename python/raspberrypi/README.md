# DFRobot_IIC_Serial.py
This is an IIC to dual UART module with 1Mbps IIC transmission rate, and each sub UART is able to receive/transmit independent 256 bytes FIFO hardware cache. It can be used in the applications requiring the transmission of large amounts of data. <br>
The band rate, word length, and check format of every sub UART can be set independently. The module can provide at most 2Mbps communication rate, and support 4 IIC addresses. Four such modules can be connected to one controller board to expand 8 hardware serial port. <br>

<br>
<img src="https://github.com/DFRobot/DFRobot_IICSerial/blob/master/resources/images/DFR0627svg.png">
<br>


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
To use this library, first download the library file, then open the examples folder and run the demo in the folder Proceed as follows:
* sudo git clone https://github.com/DFRobot/DFRobot_IICSerial
* cd python
* cd raspberrypi
* cd examples
* python demo_*


## Methods

```C++
'''
  @brief Constructor
  @param sub_uart_channel sub UART channel, WK2132 has two sub UARTs: SUBUART_CHANNEL_1 or SUBUART_CHANNEL_2
  @Parame IA1:  corresponds with IA1 Level(0 or 1) of DIP switch on the module, and is used for configuring 
  @n the IIC address of the 6th bit value(default: 1).
  @Parame IA0:  corresponds with IA0 Level(0 or 1) of DIP switch on the module, and is used for configuring
  @n IIC address of the 5th bit value(default: 1).
  @n IIC address configuration: 
  7   6   5   4   3   2   1   0
  0  IA1 IA0  1   0  C1  C0  0/1
  @n @n IIC address only has 7 bits, while there are 8 bits for one byte, so the extra one bit will be filled as 0. 
  @n The 6th bit corresponds with IA1 Level of DIP switch, can be configured manually.
  @n The 5th bit corresponds with IA0 Level of DIP switch, can be configured manually. 
  @n The 4th and 3rd bits are fixed, value 1 and 0 respectively.
  @n The values of the 2nd and 1st bits are the sub UART channels, 00 for sub UART 1, 01 for sub UART 2. 
  @n The 0 bit represents the operation object: 0 for register, 1 for FIFO cache.
'''
def __init__(self, sub_uart_channel, IA1 = 1, IA0 =1):

'''
  @brief Init function, set sub UART band rate, data format 
  @param baud: baud rate, it support: 9600, 57600, 115200, 2400, 4800, 7200,
  @n     14400, 19200, 28800,38400, 76800, 153600, 230400, 460800, 307200, 921600
  @param format: Data format, it support:
  @n     IIC_SERIAL_8N1, IIC_SERIAL_8N2, IIC_SERIAL_8Z1,IIC_SERIAL_8Z2
  @n     IIC_SERIAL_8O1, IIC_SERIAL_8O2, IIC_SERIAL_8E1, IIC_SERIAL_8E2
  @n     IIC_SERIAL_8F1, IIC_SERIAL_8F2
  @return Return 0 if it sucess, otherwise return non-zero
'''
def begin(self, baud, format = IIC_Serial_8N1):

'''
  @brief Release sub UART to clean up all registers in Sub UART. Call function begin() again to make it work.
'''
def end(self):

'''
  @The Prints the values to a stream, usage is the same as print function.
  @param sep: string inserted between values, default a space.
  @param end: string appended after the last value, default a newline.
'''
def printf(self, *args, **kargs):

'''
  @brief Get the number of bytes in receive buffer, it should be the total number of bytes in FIFO
  @n receive buffer(256B) and self-defined _rx_buffer(31B).
  @return Return the number of bytes in receive buffer
'''
def available(self):

'''
  @brief Return the data of 1 byte without deleting the data in the receive buffer
  @return Return the readings
'''
def peek(self):

'''
  @brief RRead size bytes from the serial port, this operation will delete the data in the buffer.
  @param size: the bytes of read
  @return less characters as requested.
'''
def read(self, size = 1):

'''
  @brief Wait for the data to be transmited completely
'''
def flush(self):

'''
  @brief Output the given byte string over the serial port.
  @param value: byte string
  @return return bytes actually written.
'''
def write(self, value):

```

## Compatibility

MCU                | SoftwareSerial | HardwareSerial |
------------------ | :----------: | :----------: |
Arduino Uno        |      √       |      X       |
Mega2560           |      √       |      √       |
Leonardo           |      √       |      √       |
ESP32              |      X       |      X       |
micro:bit          |      X       |      X       |
FireBeetle M0      |      X       |      X       |
raspberry          |      X       |      √       |

## History

- Data 2021-05-19
- Version V1.0

## Credits

Written by(xue.peng@dfrobot.com), 2021. (Welcome to our [website](https://www.dfrobot.com/))





