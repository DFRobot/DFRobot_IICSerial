from __future__ import print_function
# -*- coding:utf-8 -*-

'''
  # demo_read_serial.py
  #
  # brief Read data from sub UART (example: UART1) 
  # Detect if there is data in UART1, if there is, read and print it out. 
  # In this demo, a USB to UART module is needed for connecting the pin RX of UART1 so as to transmit data to UART1 pin RX via Serial.
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

iic_uart1 = DFRobot_IIC_Serial(sub_uart_channel = SUBUART_CHANNEL_1, IA1 = 1, IA0 = 1) #using UART2 interface of IIC to dual uart moudle.

if __name__ == "__main__":
  print("Initialization iic to dual uart moudle...", end=" ")
  while iic_uart1.begin(baud = 115200, format = IIC_Serial_8N1) != 0:
    print("failed, please check if the connection is correct?")
    time.sleep(1)
    print("Initialization iic to dual uart moudle...", end=" ")
  print("done")
  
  print("\n+------------------------------------------------+");
  print("|  Read UART1's data.                            |");
  print("|  Waiting for UART1's receive data.             |");
  print("|  Print UART1's receive data.                   |");
  print("+------------------------------------------------+");
  print("Please send data to UART1 by serial!");
  print("Waiting for data from UART1...");
  
  while True:
    while iic_uart1.available():
      c = iic_uart1.read()
      print(c,end='')
      sys.stdout.flush()
      time.sleep(0.002)
    time.sleep(1)

