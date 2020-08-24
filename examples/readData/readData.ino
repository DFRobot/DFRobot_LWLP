/*!
 * @file readData.ino
 * @brief Read pressure difference and temperature value of the differential pressure sensor 
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [fengli](li.feng@dfrobot.com)
 * @version  V1.0
 * @date  2020-05-14
 * @get from https://www.dfrobot.com
 * @url https://github.com/DFRobot/DFRobot_LWLP
 */
#include <DFRobot_LWLP.h>

/*! 
 * @brief Construct the function
 * @param pWire IC bus pointer object and construction device, can both pass or not pass parameters, Wire in default.
 * @param address Chip IIC address, addresses 0x0 
 */
DFRobot_LWLP lwlp;


void setup() {

  Serial.begin(9600);
  //Init Chip
  while (lwlp.begin() != 0) {
  Serial.println("Failed to initialize the chip, please confirm the chip connection");
  delay(1000);
  }
}
void loop(void){
  DFRobot_LWLP::sLwlp_t data;
  //Get data of single measurement 
  data = lwlp.getData();
  //Get data processed by the filter 
  //data = lwlp.getfilterData();
  //Get temperature in unit centigrade degree
  Serial.print("Temperature: ");
  Serial.print(data.temperature);
  Serial.println(" C");
  Serial.print("Differential Pressure: ");
  //Get pressure difference in unit pa 
  Serial.print(data.presure);
  Serial.println(" pa");
  delay(500);

}
