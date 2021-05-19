from __future__ import print_function
# -*- coding:utf-8 -*-

'''
  # demo_write_serial.py
  #
  # brief Write data into sub UART (example: UART2)
  # @n Experiment phenomenon: transmit a data from Sub UART2 every 2s. 
  # @n In this demo, a USB to UART module is needed for connecting the pin TX of UART2 so as to check print information via serial.
  #
  # @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  # @licence     The MIT License (MIT)
  # @author [Arya](xue.peng@dfrobot.com)
  # @version  V1.0
  # @date  2021-05-17
  # @get from https://www.dfrobot.com
  # @url https://github.com/DFRobot/DFRobot_IICSerial
'''

import sys
import os
import time

sys.path.append(os.path.dirname(os.path.dirname(os.path.realpath(__file__))))
from DFRobot_IIC_Serial import *

iic_uart2 = DFRobot_IIC_Serial(sub_uart_channel = SUBUART_CHANNEL_2, IA1 = 1, IA0 = 1) #using UART2 interface of IIC to dual uart moudle.

if __name__ == "__main__":
  print("Initialization iic to dual uart moudle...", end=" ")
  while iic_uart2.begin(baud = 115200, format = IIC_Serial_8N1) != 0:
    print("failed, please check if the connection is correct?")
    time.sleep(1)
    print("Initialization iic to dual uart moudle...", end=" ")
  print("done")

  print("\n+--------------------------------------------------------------+");
  print("|  Write data to UART2.                                        |");
  print("|  Connected UART2's TX pin to a UART module!                  |");#Connect UART pin TX to pin RX of USB to UART module
  print("|  UART2 send data to serial                                   |");
  print("+--------------------------------------------------------------+");
  print("Start to send data to serial.");
  
  while True:
    name = "Tom"
    age = 15
    price = 3.5
    iic_uart2.printf("hello, world!")
    iic_uart2.printf("How to spell apple?", end='\r\n')
    iic_uart2.printf('a','p','p','l','e', sep = '_', end = '\r\n')
    iic_uart2.printf("data:", "2021.05.09")
    iic_uart2.printf("His name is %s"%name, "His age is  %d"%15, sep = '\n')
    iic_uart2.printf("The banana's price is %f a bound"%price)
    iic_uart2.write('12345\n')
    iic_uart2.write(['a','p','p','l','e','\n'])
    iic_uart2.write((49, 50, 51, '\n','\n'))
    time.sleep(1)

