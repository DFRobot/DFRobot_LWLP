/*!
 * @file DFRobot_LWLP.h
 * @brief Define the infrastructure of the DFRobot_LWLP class
 * @details The differential pressure between two probes of the \
 * @n differential pressure sensor can be obtained by this drive
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @License     The MIT License (MIT)
 * @author [fengli](li.feng@dfrobot.com)
 * @version  V1.0
 * @date  2020-05-14
 * @url https://github.com/DFRobot/DFRobot_LWLP
 */
 
#ifndef DFROBOT_LWLP_H
#define DFROBOT_LWLP_H
#include "Arduino.h"
#include <Wire.h>

//#define ENABLE_DBG

#ifdef ENABLE_DBG
#define DBG(...) {Serial.print("[");Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
#define DBG(...)
#endif


#define SENSOR_TYPE    LWPX  ///<Sensor type 
#define MAX_PRESURE    600 ///<Maximum value of pressure range(Pa)
#define MIN_PRESURE    600 ///<Minimum value of pressure range(Pa)

#define MAX_TEMP       85.0 ///<Highest value of temperature range(℃)
#define MIN_TEMP       40.0 ///<Lowest value of temperature range(℃)



class DFRobot_LWLP
{
public:
  #define ERR_OK             0      ///<No error
  #define ERR_DATA_BUS      -1      ///<Data bus error
  #define ERR_IC_VERSION    -2      ///<Chip version does not match
  
  /**
   * @struct sLwlp_t
   */ 
  typedef struct
  {
    uint8_t  status;/**<Status Bit>*/
    uint8_t  presureDataH;/**<Sensor orignal value high byte>*/
    uint8_t  presureDataM;/**<Sensor orignal value middle byte>*/
    uint8_t  presureDataL;/**<Sensor orignal value low byte>*/
    uint8_t  tempDataH;/**<Temperature orignal value high byte>*/
    uint8_t  tempDataM;/**<Temperature orignal value middle byte>*/
    uint8_t  tempDataL;/**<Temperature orignal value low byte>*/
    uint16_t  presureData;/**<Combined pressure orignal value>*/
    uint16_t  tempData;/**<Combined temperature orignal value>*/
    float  presure;/**<Convert pressure variable>*/
    float  temperature;/**<Convert temperature variable>*/
    float  presureFilter[5];    /**<Pressure orignal value>*/
    float  presureFilterMem;/**<Pressure sorting middle value>*/
    double  presureFilterSum;/**<Sorting and accumulating>*/
    float  presureFilterResult;/**<Final value processed by filter>*/
    float  temperatureFilter[5];    /**<Temperature orignal value>*/
    float  temperatureFilterMem;/**<Temperature sorting middle value>*/
    double  temperatureFilterSum;/**<Sorting and accumulating>*/
    float  temperatureFilterResult;/**<Final value processed by filter>*/
    float  presureDrift;
  }sLwlp_t ;
  
  /**
   * @fn DFRobot_LWLP
   * @brief Construct the function
   * @param pWire IC bus pointer object and construction device, can both pass or not pass parameters, Wire in default.
   */
  DFRobot_LWLP(TwoWire *pWire = &Wire);

  /**
   * @fn begin
   * @brief Initialize the function
   * @return Return 0 indicates a successful initialization, while other values indicates failure and return to error code.
   */
  int begin();
  /**
   * @fn getData
   * @brief Get sensor data of sigle measurement
   * @return Data of struct type 
   */
  sLwlp_t getData(void);

  /**
   * @brief Send a measurement command without waiting for conversion.
   * @return true on success; false if a measurement is pending or I2C fails.
   * @note I2C transactions are synchronous; no conversion delay is added.
   */
  bool startMeasurement(void);

  /**
   * @brief Return true when 30 ms have elapsed since a successful start.
   * @note Checks elapsed time only; does not access the sensor or I2C bus.
   */
  bool dataReady(void);

  /**
   * @brief Read a ready measurement without waiting for conversion.
   * @return Latest cached data if called early, without a start, or on I2C error.
   * @note Use readData(data) when the caller needs explicit success reporting.
   */
  sLwlp_t readData(void);

  /**
   * @brief Read a ready measurement into data.
   * @return true on a complete read; otherwise false, leaving data unchanged.
   * @note An early call keeps the measurement pending. An attempted I2C read
   * consumes it, including on error; start another measurement to retry.
   */
  bool readData(sLwlp_t &data);

  /**
   * @fn getfilterData
   * @brief Filter processing funcition, get sensor data processed by this function 
   * @return Data of struct type 
   */
  sLwlp_t getfilterData(void);
  
private:
  sLwlp_t  lwlp;
  bool _measurementPending;
  uint32_t _measurementStarted;
  TwoWire *_pWire;
  uint8_t _address = 0x00;

  /**
   * @fn readData
   * @brief Write command to sensor chip.
   * @param pBuf  The data contained in the command.
   * @param size  Number of command data
   * @return Return 0 indicates the successful read, other return values suggest unsuccessful read.
   */
  uint8_t readData(void *pBuf,size_t size);
};   
#endif
