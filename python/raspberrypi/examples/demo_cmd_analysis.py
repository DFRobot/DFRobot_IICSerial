from __future__ import print_function
# -*- coding:utf-8 -*-

'''
  # demo_cmd_analysis.py
  #
  # brief Analyze UART command, save and print (example: UART2, connect UART2's RX and TX together)
  # Transmit a random string via UART: "ABCDEFASFGHJUAAAEEB"
  # Receive the string, remove the char "A" of the string, and then print out the new string.
  #
  # @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  # @licence     The MIT License (MIT)
  # @author [Arya](xue.peng@dfrobot.com)
  # @version  V1.0
  # @date  2021-05-19
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

  print("\n+-----------------------------------------------------+");
  print("|  connected UART2's TX pin to RX pin.                |");
  print("|  Analysis string and eliminate a char of a string.  |");
  print("|  Original string: ABCDEFASFGHJUAAAEEB               |");
  print("|  Eliminate char: A                                  |");
  print("|  Original string: BCDEFSFGHJUEEB                    |");
  print("|  Print the parsed string.                           |");
  print("+-----------------------------------------------------+");
  print("Please Send to the string by UART2's TX.");
  print("UART2 send a string: ABCDEFASFGHJUAAAEEB");
  iic_uart2.printf("ABCDEFASFGHJUAAAEEB");
  
  while True:
    length = iic_uart2.available()
    i = 0
    rx_data = ''
    if length:
      while(iic_uart2.available()):
        if iic_uart2.peek() != 'A': 
          rx_data += iic_uart2.read()
        else:
          iic_uart2.read()
      print("Parsed string: %s"%rx_data)
    time.sleep(1)

