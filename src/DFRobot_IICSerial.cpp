/*!
 * @file DFRobot_IICSerial.h
 * @brief Define the basic structure of class DFRobot_IICSerial
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2019-07-28
 * @https://github.com/DFRobot/DFRobot_IICSerial
 */
#include <Arduino.h>
#include <DFRobot_IICSerial.h>

/**
 * @brief Global register
 */
#define REG_WK2132_GENA   0x00   //< Global control register, control sub UART clock 
#define REG_WK2132_GRST   0x01   //< Global sub UART reset register, reset a sub UART independently through software
#define REG_WK2132_GMUT   0x02   //< Global main UART control register, and will be used only when the main UART is selected as UART, no need to be set here.
#define REG_WK2132_GIER   0x10   //< Global interrupt register, control sub UART total interrupt.
#define REG_WK2132_GIFR   0x11   //< Global interrupt flag register, only-read register: indicate if there is a interrupt occuring on a sub UART.

/**
 * @brief Page control register
 */
#define REG_WK2132_SPAGE  0x03   //< sub UART page control register

/**
 * @brief sub UART register SPAGE0
 */
#define REG_WK2132_SCR    0x04   //< Sub UART control register
#define REG_WK2132_LCR    0x05   //< Sub UART configuration register
#define REG_WK2132_FCR    0x06   //< Sub UART FIFO control register
#define REG_WK2132_SIER   0x07   //< Sub UART interrupt enable register
#define REG_WK2132_SIFR   0x08   //< Sub UART interrupt flag register 
#define REG_WK2132_TFCNT  0x09   //< Sub UART transmit FIFO register, OR register
#define REG_WK2132_RFCNT  0x0A   //< Sub UART transmit FIFO register, OR register 
#define REG_WK2132_FSR    0x0B   //< Sub UART FIFO register, OR register 
#define REG_WK2132_LSR    0x0C   //< Sub UART receive register, OR register 
#define REG_WK2132_FDAT   0x0D   //< Sub UART FIFO data register 
/**
 * @brief Sub UART register SPAGE1
 */
#define REG_WK2132_BAUD1  0x04   //< Sub UART band rate configuration register high byte
#define REG_WK2132_BAUD0  0x05   //< Sub UART band rate configuration register low byte
#define REG_WK2132_PRES   0x06   //< Sub UART band rate configuration register decimal part
#define REG_WK2132_RFTL   0x07   //< Sub UART receive FIFO interrupt trigger configuration register
#define REG_WK2132_TFTL   0x08   //< Sub UART transmit FIFO interrupt trigger configuration register

DFRobot_IICSerial::DFRobot_IICSerial(TwoWire &wire,  uint8_t subUartChannel, uint8_t IA1, uint8_t IA0){
  _pWire = &wire;
  _addr = (IA1 << 6) | (IA0 << 5) | IIC_ADDR_FIXED;
  _subSerialChannel = subUartChannel;
  _rx_buffer_head = 0;
  _rx_buffer_tail = 0;
  memset(_rx_buffer, 0, sizeof(_rx_buffer));
}

DFRobot_IICSerial::~DFRobot_IICSerial(){
  
}

int DFRobot_IICSerial::begin(long unsigned baud, uint8_t format, eCommunicationMode_t mode, eLineBreakOutput_t opt){
  _rx_buffer_head = _rx_buffer_tail;
  _pWire->begin();
  uint8_t val = 0;
  uint8_t channel = subSerialChnnlSwitch(SUBUART_CHANNEL_1);
  if(readReg(REG_WK2132_GENA, &val, 1) != 1){
      DBG("READ BYTEERROR!");
      return ERR_READ;
  }
#ifndef ARDUINO_ARCH_NRF5
  if((val & 0x80) == 0){
      DBG("Read REG_WK2132_GENA  ERROR!");
      return ERR_REGDATA;
  }
#endif
  subSerialChnnlSwitch(channel);
  subSerialConfig(_subSerialChannel);
  DBG("OK");
  setSubSerialBaudRate(baud);
  setSubSerialConfigReg(format, mode, opt);
  return ERR_OK;
}

void DFRobot_IICSerial::end(){
  subSerialGlobalRegEnable(_subSerialChannel, rst);
}

int DFRobot_IICSerial::available(void){
  int index;
  uint8_t val = 0;
  sFsrReg_t fsr;
  if(readReg(REG_WK2132_RFCNT, &val, 1) != 1){
      DBG("READ BYTE SIZE ERROR!");
      return 0;
  }
  index = (int)val;
  if(index == 0){
      fsr = readFIFOStateReg();
      if(fsr.rDat == 1){
          index = 256;
      }
  }
  return (index + ((unsigned int)(SERIAL_RX_BUFFER_SIZE + _rx_buffer_head - _rx_buffer_tail)) % SERIAL_RX_BUFFER_SIZE);
}

int DFRobot_IICSerial::peek(void){
  int num = available() - (((unsigned int)(SERIAL_RX_BUFFER_SIZE + _rx_buffer_head - _rx_buffer_tail)) % SERIAL_RX_BUFFER_SIZE);
  for(int i = 0; i < num; i++){
      rx_buffer_index_t j = (rx_buffer_index_t)(_rx_buffer_head + 1) % SERIAL_RX_BUFFER_SIZE;
      if(j != _rx_buffer_tail){
          uint8_t val = 0;
          readReg(REG_WK2132_FDAT, &val, 1);
          _rx_buffer[_rx_buffer_head] = val;
          _rx_buffer_head = j;
      }else{
          break;
      }
  }
  if(_rx_buffer_head == _rx_buffer_tail){
      return -1;
  }
  return _rx_buffer[_rx_buffer_tail];
}

int DFRobot_IICSerial::read(void){
  int num = available() - ((unsigned int)(SERIAL_RX_BUFFER_SIZE + _rx_buffer_head - _rx_buffer_tail)) % SERIAL_RX_BUFFER_SIZE;
  for(int i = 0; i < num; i++){
      rx_buffer_index_t j = (rx_buffer_index_t)(_rx_buffer_head + 1) % SERIAL_RX_BUFFER_SIZE;
      if(j != _rx_buffer_tail){
          uint8_t val = 0;
          readReg(REG_WK2132_FDAT, &val, 1);
          _rx_buffer[_rx_buffer_head] = val;
          _rx_buffer_head = j;
      }else{
          break;
      }
  }
  if(_rx_buffer_head == _rx_buffer_tail){
      return -1;
  }
  unsigned char c = _rx_buffer[_rx_buffer_tail];
  _rx_buffer_tail = (rx_buffer_index_t)(_rx_buffer_tail + 1) % SERIAL_RX_BUFFER_SIZE;
  return c;
}

size_t DFRobot_IICSerial::write(uint8_t value){
  sFsrReg_t fsr;
  fsr = readFIFOStateReg();
  if(fsr.tFull == 1){
      DBG("FIFO full!");
      return -1;
  }
  writeReg(REG_WK2132_FDAT, &value, 1);
  return 1;
}

size_t DFRobot_IICSerial::write(const uint8_t *pBuf, size_t size){
  if(pBuf == NULL){
    DBG("pBuf ERROR!! : null pointer");
    return 0;
  }
  uint8_t *_pBuf = (uint8_t *)pBuf;
  sFsrReg_t fsr;
  uint8_t val = 0;
  fsr = readFIFOStateReg();
  if(fsr.tFull == 1){
      DBG("FIFO full!");
      return 0;
  }
  writeFIFO(_pBuf, size);
  return size;
}

size_t DFRobot_IICSerial::read(void *pBuf, size_t size){
  if(pBuf == NULL){
    DBG("pBuf ERROR!! : null pointer");
    return 0;
  }
  uint8_t *_pBuf = (uint8_t *)pBuf;
  size = available() - (((unsigned int)(SERIAL_RX_BUFFER_SIZE + _rx_buffer_head - _rx_buffer_tail)) % SERIAL_RX_BUFFER_SIZE);
  readFIFO(_pBuf, size);
  return size;
}
void DFRobot_IICSerial::flush(void){
  sFsrReg_t fsr = readFIFOStateReg();
  while(fsr.tDat == 1);
}


void DFRobot_IICSerial::subSerialConfig(uint8_t subUartChannel){
  DBG("Sub UART clock enable");
  subSerialGlobalRegEnable(subUartChannel, clock);
  DBG("Software reset sub UART");
  subSerialGlobalRegEnable(subUartChannel, rst);
  DBG("Sub UART global interrupt enable");
  subSerialGlobalRegEnable(subUartChannel, intrpt);
  DBG("Sub UART page register setting (default PAGE0)");
  subSerialPageSwitch(page0);
  DBG("Sub interrupt setting");
  sSierReg_t sier = {.rFTrig = 0x01, .rxOvt = 0x01, .tfTrig = 0x01, .tFEmpty = 0x01, .rsv = 0x00, .fErr = 0x01};
  subSerialRegConfig(REG_WK2132_SIER, &sier);
  DBG("enable transmit/receive FIFO");
  sFcrReg_t fcr = {.rfRst = 0x01, .tfRst = 0x00, .rfEn = 0x01, .tfEn = 0x01, .rfTrig = 0x00, .tfTrig = 0x00};
  subSerialRegConfig(REG_WK2132_FCR, &fcr);
  DBG("Sub UART reiceive/transmit enable");
  sScrReg_t scr = {.rxEn = 0x01, .txEn = 0x01, .sleepEn = 0x00, .rsv = 0x00 };
  subSerialRegConfig(REG_WK2132_SCR, &scr);
}

void DFRobot_IICSerial::subSerialGlobalRegEnable(uint8_t subUartChannel, eGlobalRegType_t type){
  if(subUartChannel > SUBUART_CHANNEL_ALL)
  {
      DBG("SUBSERIAL CHANNEL NUMBER ERROR!");
      return;
  }
  uint8_t val = 0;
  uint8_t regAddr = getGlobalRegType(type);
  uint8_t channel = subSerialChnnlSwitch(SUBUART_CHANNEL_1);
  DBG("reg");DBG(regAddr, HEX);
  if(readReg(regAddr, &val, 1) != 1){
        DBG("READ BYTE SIZE ERROR!");
      return;
  }
  DBG("before:");DBG(val, HEX);
  switch(subUartChannel){
      case SUBUART_CHANNEL_1:
                             val |= 0x01;
                             break;
      case SUBUART_CHANNEL_2:
                             val |= 0x02;
                             break;
      default:
              val |= 0x03;
              break;
  }
  writeReg(regAddr, &val, 1);
  readReg(regAddr, &val, 1);
  DBG("after:");DBG(val, HEX);
  subSerialChnnlSwitch(channel);
}

void DFRobot_IICSerial::subSerialPageSwitch(ePageNumber_t page){
  if(page >= pageTotal){
      return;
  }
  uint8_t val = 0;
  if(readReg(REG_WK2132_SPAGE, &val, 1) != 1){
      DBG("READ BYTE SIZE ERROR!");
      return;
  }
  switch(page){
      case page0:
                 val &= 0xFE;
                 break;
      case page1:
                 val |= 0x01;
                 break;
      default:
              break;
  }
  DBG("before: "); DBG(val);
  writeReg(REG_WK2132_SPAGE, &val, 1);
  readReg(REG_WK2132_SPAGE, &val, 1);
  DBG("after: ");DBG(val, HEX);
}

void DFRobot_IICSerial::subSerialRegConfig(uint8_t reg, void *pValue){
  uint8_t val = 0;
  readReg(reg, &val, 1);
  DBG("before: "); DBG(val);
  val |= *(uint8_t *)pValue;
  writeReg(reg, &val, 1);
  readReg(reg, &val, 1);
  DBG("after: ");DBG(val, HEX);
}

uint8_t DFRobot_IICSerial::getGlobalRegType(eGlobalRegType_t type){
  if((type < clock) || (type > intrpt)){
      DBG("Global Reg Type Error!");
      return 0;
  }
  uint8_t regAddr = 0;
  switch(type){
      case clock:
                 regAddr = REG_WK2132_GENA;
                 break;
      case rst:
                 regAddr = REG_WK2132_GRST;
                 break;
      default:
              regAddr = REG_WK2132_GIER;
              break;
  }
  return regAddr;
}

void DFRobot_IICSerial::setSubSerialBaudRate(unsigned long baud){
  uint8_t scr = 0x00,clear = 0x00;
  readReg(REG_WK2132_SCR, &scr, 1);
  subSerialRegConfig(REG_WK2132_SCR, &clear);
  uint8_t baud1 = 0,baud0 = 0, baudPres = 0;
  uint16_t valIntger  = FOSC/(baud * 16) - 1;
  uint16_t valDecimal = (FOSC%(baud * 16))/(baud * 16); 
  baud1 = (uint8_t)(valIntger >> 8);
  baud0 = (uint8_t)(valIntger & 0x00ff);
  while(valDecimal > 0x0A){
      valDecimal /= 0x0A;
  }
  baudPres = (uint8_t)(valDecimal);
  subSerialPageSwitch(page1);
  subSerialRegConfig(REG_WK2132_BAUD1, &baud1);
  subSerialRegConfig(REG_WK2132_BAUD0, &baud0);
  subSerialRegConfig(REG_WK2132_PRES, &baudPres);

  readReg(REG_WK2132_BAUD1, &baud1, 1);
  readReg(REG_WK2132_BAUD0, &baud0, 1);
  readReg(REG_WK2132_PRES, &baudPres, 1);
  DBG(baud1, HEX);
  DBG(baud0, HEX);
  DBG(baudPres, HEX);
  subSerialPageSwitch(page0);
  subSerialRegConfig(REG_WK2132_SCR, &scr);
}

void DFRobot_IICSerial::setSubSerialConfigReg(uint8_t format, eCommunicationMode_t mode, eLineBreakOutput_t opt){
  uint8_t _mode = (uint8_t)mode;
  uint8_t _opt = (uint8_t)opt;
  uint8_t val = 0;
  _addr = updateAddr(_addr, _subSerialChannel, OBJECT_REGISTER);
  if(readReg(REG_WK2132_LCR, &val, 1) != 1){
      DBG("Read Byte ERROR！");
      return;
  }
  DBG("before: "); DBG(val, HEX);
  sLcrReg_t lcr = *((sLcrReg_t *)(&val));
  lcr.format = format;
  lcr.irEn = _mode;
  lcr.lBreak = _opt;
  val = *(uint8_t *)&lcr;
  writeReg(REG_WK2132_LCR, &val, 1);
  readReg(REG_WK2132_LCR, &val, 1);
  DBG("after: "); DBG(val, HEX);
}

uint8_t DFRobot_IICSerial::updateAddr(uint8_t pre, uint8_t subUartChannel, uint8_t obj){
  sIICAddr_t addr ={.type = obj, .uart = subUartChannel, .addrPre = (uint8_t)((int)pre >> 3)};
  return *(uint8_t *)&addr;
}

DFRobot_IICSerial::sFsrReg_t DFRobot_IICSerial::readFIFOStateReg(){
  sFsrReg_t fsr;
  readReg(REG_WK2132_FSR, &fsr, sizeof(fsr));
  return fsr;
}

uint8_t DFRobot_IICSerial::subSerialChnnlSwitch(uint8_t subUartChannel){
  uint8_t channel = _subSerialChannel;
  _subSerialChannel = subUartChannel;
  return channel;
}
void DFRobot_IICSerial::sleep(){
  
}

void DFRobot_IICSerial::wakeup(){

}
void DFRobot_IICSerial::writeReg(uint8_t reg, const void* pBuf, size_t size){
  if(pBuf == NULL){
      DBG("pBuf ERROR!! : null pointer");
  }
  _addr = updateAddr(_addr, _subSerialChannel, OBJECT_REGISTER);
  uint8_t * _pBuf = (uint8_t *)pBuf;
  _pWire->beginTransmission(_addr);
  _pWire->write(&reg, 1);

  for(uint16_t i = 0; i < size; i++){
    _pWire->write(_pBuf[i]);
  }
  _pWire->endTransmission();
}

uint8_t DFRobot_IICSerial::readReg(uint8_t reg, void* pBuf, size_t size){
  if(pBuf == NULL){
    DBG("pBuf ERROR!! : null pointer");
    return 0;
  }
  _addr = updateAddr(_addr, _subSerialChannel, OBJECT_REGISTER);
  uint8_t * _pBuf = (uint8_t *)pBuf;
  _addr &= 0xFE;
  _pWire->beginTransmission(_addr);
  _pWire->write(&reg, 1);
  if(_pWire->endTransmission() != 0){
      return 0;
  }
  _pWire->requestFrom(_addr, (uint8_t) size);
  for(uint16_t i = 0; i < size; i++){
    _pBuf[i] = (char)_pWire->read();
  }
  return size;
}

uint8_t DFRobot_IICSerial::readFIFO(void* pBuf, size_t size){
  if(pBuf == NULL){
    DBG("pBuf ERROR!! : null pointer");
    return 0;
  }
  _addr = updateAddr(_addr, _subSerialChannel, OBJECT_FIFO);
  uint8_t *_pBuf = (uint8_t *)pBuf;
  size_t left = size,num = 0;
  while(left){
      num = (left > IIC_BUFFER_SIZE) ?  IIC_BUFFER_SIZE : left;
      _pWire->beginTransmission(_addr);
      if(_pWire->endTransmission() != 0){
          return 0;
      }
      _pWire->requestFrom(_addr, (uint8_t) num);
      for(size_t i = 0; i < num; i++){
          *(_pBuf+i) = _pWire->read();
      }
      left -=num;
      _pBuf += num;
  }
  return (uint8_t)size;
}
void DFRobot_IICSerial::writeFIFO(void *pBuf, size_t size){
  if(pBuf == NULL){
      DBG("pBuf ERROR!! : null pointer");
      return;
  }
  _addr = updateAddr(_addr, _subSerialChannel, OBJECT_FIFO);
  uint8_t *_pBuf = (uint8_t *)pBuf;
  size_t left = size;
  while(left){
      size = (left > IIC_BUFFER_SIZE) ? IIC_BUFFER_SIZE: left;
      _pWire->beginTransmission(_addr);
      _pWire->write(_pBuf, size);
      if(_pWire->endTransmission() != 0){
          return;
      }
      delay(10);
      left -= size;
      _pBuf = _pBuf + size;
  }
}
