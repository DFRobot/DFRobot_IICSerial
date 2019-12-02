/*!
 * @file cmdAnalysis.ino
 * @brief Analyze UART command, save and print (example: UART2, connect UART2's RX and TX together)
 * @n Transmit a random string via UART: "ABCDEFASFGHJUAAAEEB"
 * @n Receive the string, remove the char "A" of the string, and then print out the new string.
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
DFRobot_IICSerial iicSerial2(Wire, /*subUartChannel =*/SUBUART_CHANNEL_2, /*IA1 = */1,/*IA0 = */1);//Construct Sub UART2

char rx_buffer[256];//Define a receive buffer to store the data received by UART2
void setup() {
  Serial.begin(115200);
  /*begin Init function, set band rate according to the selected crystal frequency.
  begin(long unsigned baud) Call the function, set sub UART band rate. 
  default setting->Band rate: baud, data format：IICSerial_8N1, 8 bits data, no check mode, 1 bit stop bit.
  begin(long unsigned baud, uint8_t format) Use the function to set band rate and data format:
  Parameter supported baud: 2400, 4800, 57600, 7200, 9600, 14400, 19200, 28800, 38400,
                76800, 115200, 153600, 230400, 460800, 307200, 921600
  Parameter available format: 
  IICSerial_8N1  IICSerial_8N2  IICSerial_8Z1  IICSerial_8Z2  IICSerial_8O1
  IICSerial_8O2  IICSerial_8E1  IICSerial_8E2  IICSerial_8F1  IICSerial_8F2
  8 represents the number of data bit, N for no parity, Z for 0 parity, O for Odd parity, E for Even parity,
  F for 1 parity, 1 or 2 for the number of stop bit. Default IICSerial_8N1
  */
  iicSerial2.begin(/*baud = */115200);/*UART2 init*/
  //iicSerial2.begin(/*baud = */115200, /*format = */IICSerial_8N1);
  Serial.println("\n+-----------------------------------------------------+");
  Serial.println("|  connected UART2's TX pin to RX pin.                |");
  Serial.println("|  Analysis string and eliminate a char of a string.  |");
  Serial.println("|  Original string: ABCDEFASFGHJUAAAEEB               |");
  Serial.println("|  Eliminate char: A                                  |");
  Serial.println("|  Original string: BCDEFSFGHJUEEB                    |");
  Serial.println("|  Print the parsed string.                           |");
  Serial.println("+-----------------------------------------------------+");
  Serial.println("Please Send to the string by UART2's TX.");
  Serial.println("UART2 send a string: ABCDEFASFGHJUAAAEEB");
  iicSerial2.println("ABCDEFASFGHJUAAAEEB");//UART2 transmit string "ABCDEFASFGHJUAAAEEB"
}

void loop() {
  int n = iicSerial2.available();//Read the number of bytes in UART2 
  int i = 0;
  if(n){
      while(iicSerial2.available()){
          if((char)iicSerial2.peek() != 'A'){//Use peek function to read the character without deleting the data in buffer.
              rx_buffer[i++] = iicSerial2.read();//Use read function to read character and delete the data in buffer.
              if((i > (sizeof(rx_buffer) - 1)))
                  break;
          }else{
              iicSerial2.read();//Put read function here to remove a char "A" in buffer.
          }
      }
      Serial.print("Parsed string: ");
      for(int j = 0; j < i; j++){
          Serial.print(rx_buffer[j]);
      }
      Serial.println();
  }
  delay(1000);
}
