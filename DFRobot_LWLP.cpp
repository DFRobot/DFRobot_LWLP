/*!
 * @file DFRobot_LWLP.cpp
 * @brief Define the infrastructure of the DFRobot_LWLP class
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [fengli](li.feng@dfrobot.com)
 * @version  V1.0
 * @date  2020-05-26
 * @get from https://www.dfrobot.com
 * @url https://github.com/DFRobot/DFRobot_LWLP
 */

#include "DFRobot_LWLP.h"

DFRobot_LWLP::DFRobot_LWLP(TwoWire *pWire)
{
  _pWire = pWire;
  
}
int DFRobot_LWLP::begin(){

  _pWire->begin();
  _pWire->beginTransmission(_address);
  if(_pWire->endTransmission() != 0){
    DBG("bus data access error");
    return ERR_DATA_BUS;
   }
   getfilterData();
   lwlp.presureDrift = lwlp.presure;
  return ERR_OK;


}
void DFRobot_LWLP::configChip(void){
  uint8_t config[3]={0xaa,0x00,0x80}; //Config list 
  write(config,3);//Transmit config data 
  delay(30);//delay 30 ms to wait the chip to collect and convert data 
}
DFRobot_LWLP::sLwlp_t DFRobot_LWLP::getData(void){//Get chip register data 
  uint8_t data[7];
  configChip();
  uint8_t error = readData(data,7);
  if(error != 7){
    DBG("data request error");
  }

  lwlp.status = data[0];//Read chip data output status 
  lwlp.presureDataH = data[1];//Pressure highest byte 
  lwlp.presureDataM = data[2];//Pressure middle byte 
  lwlp.presureDataL = data[3];//Pressure lowest byte 
  lwlp.tempDataH = data[4];//Temperature highest byte 
  lwlp.tempDataM = data[5];//Temperature middle byte   
  lwlp.tempDataL = data[6];//Temperature lowest byte 
  
  
  lwlp.presureData = (lwlp.presureDataH<<8)+(lwlp.presureDataM);
  lwlp.presureData=lwlp.presureData>>2;

  lwlp.presure=(lwlp.presureData/16384.0)*(MAX_PRESURE+MIN_PRESURE)-MIN_PRESURE - lwlp.presureDrift;
  lwlp.tempData = 0 ;
  lwlp.tempData = (lwlp.tempDataH<<8)+(lwlp.tempDataM);
  lwlp.temperature=(lwlp.tempData/65536.0)*(MAX_TEMP+MIN_TEMP)-MIN_TEMP;
  
  return lwlp;
  }
DFRobot_LWLP::sLwlp_t DFRobot_LWLP::getfilterData(void){
  uint8_t readTime;
  uint8_t i,j,k;
 
  for(readTime=0;readTime<5;readTime++){;
            getData();
            lwlp.presureFilter[readTime]=lwlp.presure;
            lwlp.temperatureFilter[readTime]=lwlp.temperature;

    }   
  
  for(i=0;i<5-1;++i)
  {
      for(j=0;j<5-i-1;++j)
      {
          if(lwlp.presureFilter[j] > lwlp.presureFilter[j+1]) 
          {
              lwlp.presureFilterMem = lwlp.presureFilter[j] ; 
              lwlp.presureFilter[j] = lwlp.presureFilter[j+1] ;  
              lwlp.presureFilter[j+1] = lwlp.presureFilterMem;  
          }
      }
  }   
      
  lwlp.presureFilterSum=0; 
  for(k=1;k<4;k++)    
  {
      lwlp.presureFilterSum=lwlp.presureFilterSum+ lwlp.presureFilter[k];
  }   
  lwlp.presure=lwlp.presureFilterSum/3;
   
  for(i=0;i<5-1;++i)
  {
      for(j=0;j<5-i-1;++j)
      {
          if(lwlp.temperatureFilter[j] > lwlp.temperatureFilter[j+1]) 
          {
              lwlp.temperatureFilterMem = lwlp.temperatureFilter[j] ; 
              lwlp.temperatureFilter[j] =  lwlp.temperatureFilter[j+1] ;  
              lwlp.temperatureFilter[j+1] =  lwlp.temperatureFilterMem;  
          }
      }
  }   
 
  lwlp.temperatureFilterSum=0; 
  for(k=1;k<4;k++)    
  {
      lwlp.temperatureFilterSum=lwlp.temperatureFilterSum + lwlp.temperatureFilter[k];
  }   
    
  lwlp.temperature=lwlp.temperatureFilterSum/3;

  return lwlp;
}


void DFRobot_LWLP::write(const void* pBuf,size_t size)
{
  if (pBuf == NULL) {
    DBG("pBuf ERROR!! : null pointer");
  }
  uint8_t * _pBuf = (uint8_t *)pBuf;
  _pWire->beginTransmission(_address);
  for (uint8_t i = 0; i < size; i++) {
    _pWire->write(_pBuf[i]);
    delay(10);
  }
  _pWire->endTransmission();
}

uint8_t DFRobot_LWLP::readData(void *pBuf, size_t size) {
  if (pBuf == NULL) {
    DBG("pBuf ERROR!! : null pointer");
  }
  uint8_t * _pBuf = (uint8_t *)pBuf;
  //Read data the chip returned
  _pWire->requestFrom(_address,size);
  uint8_t len = 0;
  for (uint8_t i = 0 ; i < size; i++) {
    _pBuf[i] = _pWire->read();
    len++;
    //Serial.println(_pBuf[i]);
  }
  _pWire->endTransmission();
  return len;
}
