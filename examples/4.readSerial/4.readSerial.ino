/*!
 * @file readSerial.ino
 * @brief Read data from sub UART (example: UART1) 
 * @n Detect if there is data in UART1, if there is, read and print it out. 
 * @n In this demo, a USB to UART module is needed for connecting the pin RX of UART1 so as to transmit data to UART1 pin RX via Serial.
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2019-07-28
 * @get from https://www.dfrobot.com
 * @url https://github.com/DFRobot/DFRobot_IICSerial
 */
#include <DFRobot_IICSerial.h>
/*DFRobot_IICSerial Constructor
 *Parameter &wire  Wire
 *Parameter subUartChannel sub UART channel, for selecting to operate UART1 or UART2
 *@n Available parameter:
     SUBUART_CHANNEL_1  SUBUART_CHANNEL_2
           UART1             UART2
 *Parameter IA1 corresponds with IA1 Level(0 or 1) of DIP switch on the module, and is used for configuring 
 * @n the IIC address of the 6th bit value(default: 1).
 *Parameter IA0 corresponds with IA0 Level(0 or 1) of DIP switch on the module, and is used for configuring
 * @n IIC address of the 5th bit value(default: 1).
 *IIC address configuration:
 * 7   6   5   4   3   2   1   0
 * 0  IA1 IA0  1   0  C1  C0  0/1
 *@n IIC address only has 7 bits, while there are 8 bits for one byte, so the extra one bit will be filled as 0. 
 *@n The 6th bit corresponds with IA1 Level of DIP switch, can be configured manually.
 *@n The 5th bit corresponds with IA0 Level of DIP switch, can be configured manually. 
 *@n The 4th and 3rd bits are fixed, value 1 and 0 respectively
 *@n The values of the 2nd and 1st bits are the sub UART channels, 00 for sub UART 1, 01 for sub UART 2. 
 *@n The 0 bit represents the operation object: 0 for register, 1 for FIFO cache.
 */
DFRobot_IICSerial iicSerial1(Wire, /*subUartChannel =*/SUBUART_CHANNEL_1, /*IA1 = */1,/*IA0 = */1);//Construct UART1
//DFRobot_IICSerial iicSerial1;//Default constructing parameter, default formal parameter: Wire SUBUART_CHANNEL_1 1 1 

void setup() {
  Serial.begin(115200);
  /*begin Init function, set band rate according to the selected crystal frequency.
  begin(long unsigned baud)Call the function, set sub UART band rate.
  default setting->Band rate: baud, data format：IICSerial_8N1, 8 bits data, no check mode, 1 bit stop bit.
  begin(long unsigned baud, uint8_t format)Use the function to set band rate and data format:
  Parameter supported baud: 2400, 4800, 57600, 7200, 9600, 14400, 19200, 28800, 38400,
                76800, 115200, 153600, 230400, 460800, 307200, 921600
  Parameter available format:
  IICSerial_8N1  IICSerial_8N2  IICSerial_8Z1  IICSerial_8Z2  IICSerial_8O1
  IICSerial_8O2  IICSerial_8E1  IICSerial_8E2  IICSerial_8F1  IICSerial_8F2
  8 represents the number of data bit, N for no parity, Z for 0 parity, O for Odd parity, E for Even parity,
  F for 1 parity, 1 or 2 for the number of stop bit. Default IICSerial_8N1
  */
  while(iicSerial1.begin(/*baud = */115200) != 0){
      Serial.println("UART init failed, please check if the connection is correct?");
      delay(10); 
 };
  //iicSerial1.begin(/*baud = */115200, /*format = */IICSerial_8N1);
  Serial.println("\n+------------------------------------------------+");
  Serial.println("|  Read UART1's data.                            |");
  Serial.println("|  Waiting for UART1's receive data.             |");
  Serial.println("|  Print UART1's receive data.                   |");
  Serial.println("+------------------------------------------------+");
  Serial.println("Please send data to UART1 by serial!");//Send data to UART1 via serial monitor.
}

void loop() {
  char c;
  if(iicSerial1.available()){/*available return the number of byte in UART1 receive buffer, none- return 0*/
    while(iicSerial1.available()){
      c = iicSerial1.read();/*Read data of UART1 receive buffer*/
      Serial.print(c);
    }
    Serial.println();
  }
  delay(1000);
}
