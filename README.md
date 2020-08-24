# DFRobot_LWLP
This high-resolution differential pressure sensor with I2C communication is used to measure the difference in pressure across two points on a device. A high-performance MEMS pressure chip and a special conditioning chip are encapsulated inside the sensor. It will give you a comparative measurement between two points. Besides, the sensor IC adopts a unique multi-stage temperature-compensation algorithm to reduce the effect of environment on the output. <br>

**NOTE**: In order to ensure the measurement resolution and the sensor life, please use the sensor in the clean air. Do not let water enter the nozzle of the sensor, or it will cause damage to the sensor.

## Table of Contents

* [Summary](#summary)
* [Installation](#installation)
* [Methods](#methods)
* [Compatibility](#compatibility)
* [History](#history)
* [Credits](#credits)

## Summary

   1.Read the pressure difference and temperature value of the differential pressure sensor <br>
   
## Installation
To use this library, please download the library file first, and paste it into the \Arduino\libraries directory, then open the examples folder and run the demo in the folder.

## Methods

```C++
  /*!
   * @brief Construct the function
   * @param pWire IC bus pointer object and construction device, can both pass or not pass parameters, Wire in default.
   */
  DFRobot_LWLP(TwoWire *pWire = &Wire);

  /**
   * @brief Initialize the function
   * @return Return 0 indicates a successful initialization, while other values indicates failure and return to error code.
   */
  int begin();
  /**
   * @brief Get sensor data of single measurement 
   * @return Data of struct type 
   */
  sLwlp_t getData(void);
  /**
   * @brief Filter processing function, get sensor data processed by this function 
   * @return Data of struct type
   */
  sLwlp_t getfilterData(void);

```

## Compatibility

MCU           | Work Well    | Work Wrong   | Untested    | Remarks
--------------| :----------: | :----------: | :---------: | -----
Arduino Uno   |      √       |              |             | 
Mega2560      |      √       |              |             | 
Leonardo      |      √       |              |             | 
ESP32         |      √       |              |             | 
micro:bit     |      √       |              |             | 


## History

- data 2019-8-25
- version V0.1


## Credits

Written by fengli(li.feng@dfrobot.com), 2019.8.25 (Welcome to our [website](https://www.dfrobot.com/))





