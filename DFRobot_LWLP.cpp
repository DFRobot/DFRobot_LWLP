/*!
 * @file DFRobot_LWLP.cpp
 * @brief Define the infrastructure of the DFRobot_LWLP class
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @License     The MIT License (MIT)
 * @author [fengli](li.feng@dfrobot.com)
 * @version  V1.0
 * @date  2020-05-14
 * @url https://github.com/DFRobot/DFRobot_LWLP
 */

#include "DFRobot_LWLP.h"

DFRobot_LWLP::DFRobot_LWLP(TwoWire *pWire)
{
  _pWire = pWire;
  lwlp = sLwlp_t();
  _measurementPending = false;
  _measurementStarted = 0;
  
}
int DFRobot_LWLP::begin(){

  _measurementPending = false;
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
bool DFRobot_LWLP::startMeasurement(void) {
  if (_measurementPending) {
    return false;
  }
  const uint8_t config[3] = {0xaa, 0x00, 0x80};
  _pWire->beginTransmission(_address);
  bool complete = true;
  for (uint8_t i = 0; i < sizeof(config); i++) {
    if (_pWire->write(config[i]) != 1) {
      complete = false;
    }
  }
  uint8_t error = _pWire->endTransmission();
  if (!complete || error != 0) {
    return false;
  }
  _measurementStarted = (uint32_t)millis();
  _measurementPending = true;
  return true;
}

bool DFRobot_LWLP::dataReady(void) {
  return _measurementPending &&
         (uint32_t)((uint32_t)millis() - _measurementStarted) >= 30;
}

DFRobot_LWLP::sLwlp_t DFRobot_LWLP::getData(void) {
  // A blocking call starts a fresh conversion, replacing any pending one.
  _measurementPending = false;
  if (!startMeasurement()) {
    return lwlp;
  }
  delay(30);
  return readData();
}

DFRobot_LWLP::sLwlp_t DFRobot_LWLP::readData(void) {
  sLwlp_t data;
  readData(data);
  return lwlp;
}

bool DFRobot_LWLP::readData(sLwlp_t &result) {
  if (!dataReady()) {
    return false;
  }
  _measurementPending = false;
  uint8_t data[7];
  if (readData(data, sizeof(data)) != sizeof(data)) {
    DBG("data request error");
    return false;
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
  
  result = lwlp;
  return true;
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


uint8_t DFRobot_LWLP::readData(void *pBuf, size_t size) {
  if (pBuf == NULL) {
    return 0;
  }
  uint8_t *buffer = (uint8_t *)pBuf;
  size_t received = _pWire->requestFrom(_address, size);
  uint8_t len = 0;
  while (len < size && len < received && _pWire->available()) {
    buffer[len++] = _pWire->read();
  }
  return len;
}
