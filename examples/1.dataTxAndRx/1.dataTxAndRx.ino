/*!
 * @file dataTxAndRx.ino
 * @brief Receive and transmit data via UART. Read the data sent by TX pin via pin RX.
 * @n Experiment phenomenon: connect the TX to RX in Sub UART1 and UART2. Read the data sent by Sub UART and print it out.
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2019-07-28
 * @url https://github.com/DFRobot/DFRobot_IICSerial
 */
#include <DFRobot_IICSerial.h>
/*DFRobot_IICSerial Constructor 
 *Parameter&wire  Wire
 *Parameter subUartChannel sub UART channel, for selecting to operate UART1 or UART2
 *@n Available parameter:
     SUBUART_CHANNEL_1  SUBUART_CHANNEL_2
           UART1             UART2
 *Parameter IA1 corresponds with IA1 Level(0 or 1) of DIP switch on the module, and is used for configuring 
 * @n the IIC address of the 6th bit value(default: 1).
 *Parameter IA0 corresponds with IA0 Level(0 or 1) of DIP switch on the module, and is used for configuring
 * @n IIC address of the 5th bit value(default: 1).
 * IIC address configuration: 
 * 7   6   5   4   3   2   1   0
 * 0  IA1 IA0  1   0  C1  C0  0/1
 *@n IIC address only has 7 bits, while there are 8 bits for one byte, so the extra one bit will be filled as 0. 
 *@n The 6th bit corresponds with IA1 Level of DIP switch, can be configured manually.
 *@n The 5th bit corresponds with IA0 Level of DIP switch, can be configured manually. 
 *@n The 4th and 3rd bits are fixed, value 1 and 0 respectively
 *@n The values of the 2nd and 1st bits are the sub UART channels, 00 for sub UART 1, 01 for sub UART 2. 
 *@n The 0 bit represents the operation object: 0 for register, 1 for FIFO cache.
 */
DFRobot_IICSerial iicSerial1(Wire, /*subUartChannel =*/SUBUART_CHANNEL_1,/*IA1 = */1,/*IA0 = */1);//Construct UART1
//DFRobot_IICSerial iicSerial1;//Default constructor, UART1, IA1 = 1, IA0 = 1
DFRobot_IICSerial iicSerial2(Wire, /*subUartChannel =*/SUBUART_CHANNEL_2, /*IA1 = */1,/*IA0 = */1);//Construct UART2

uint8_t flag = 0;//A flag bit, judge whether to print the prompt information of UART1 and UART2.
//if it is 0, print "UART1 receive data: " or "UART2 receive data: "
void setup() {
  Serial.begin(115200);
  /*begin Init function, set band rate according to the selected crystal frequency.
  begin(long unsigned baud) Call the function, set sub UART band rate.
  default setting->Band rate: baud, data format：IICSerial_8N1, 8 bits data, no check mode, 1 bit stop bit.
  begin(long unsigned baud, uint8_t format)Use the function to set band rate and data format:
  Parameter supported baud:2400, 4800, 57600, 7200, 9600, 14400, 19200, 28800, 38400, 
                76800, 115200, 153600, 230400, 460800, 307200, 921600
  Parameter available format: 
  IICSerial_8N1  IICSerial_8N2  IICSerial_8Z1  IICSerial_8Z2  IICSerial_8O1
  IICSerial_8O2  IICSerial_8E1  IICSerial_8E2  IICSerial_8F1  IICSerial_8F2
  8 represents the number of data bit, N for no parity, Z for 0 parity, O for Odd parity, E for Even parity,
  F for 1 parity, 1 or 2 for the number of stop bit. Default IICSerial_8N1
  */
  iicSerial1.begin(/*baud = */115200);/*UART1 init*/
  //iicSerial1.begin(/*baud = */115200, /*format = */IICSerial_8N1);

  iicSerial2.begin(/*baud = */115200);/*UART2 init*/
  Serial.println("\n+--------------------------------------------+");
  Serial.println("|  Connected UART1's TX pin to RX pin.       |");//Connect pin TX and RX of UART1 
  Serial.println("|  Connected UART2's TX pin to RX pin.       |");//Connect pin TX and RX of UART2
  Serial.println("|  UART1 send a String: \"hello, Serial1!\"    |");//UART1 transmit a string "hello, Serial1!"
  Serial.println("|  UART2 send a number: 123                  |");//UART2 transmit numbers 123
  Serial.println("+--------------------------------------------+");
  iicSerial1.println("hello, Serial1!");//UART1 transmit string:"hello, Serial1!"
  iicSerial2.write(123);//UART2 transmit:123
  Serial.println("Serial to print UART1 and UART2's receive data.");//print the data received by UART1 and UART2
}

void loop() {
  char c;
  if(iicSerial1.available()){/*available return the number of byte in UART1 receive buffer, none- return 0*/
    flag = 0;
    while(iicSerial1.available()){
      if(flag == 0){
        Serial.print("\nUART1 receive data: ");
        flag = 1;
      }
      c = iicSerial1.read();/*Read data of UART1 receive buffer */
      Serial.print(c);
    }
  }
  if(iicSerial2.available()){
    flag = 0;
    while(iicSerial2.available()){
      if(flag == 0){
        Serial.print("\nUART2 receive data: ");
        flag = 1;
      }
      Serial.print(iicSerial2.read());/*Read and print the data of Sub UART2 receive buffer*/
    }
  }
  delay(1000);
}
