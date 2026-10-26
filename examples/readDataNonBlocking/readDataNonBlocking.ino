/*!
 * @file readDataNonBlocking.ino
 * @brief Keep loop responsive while the sensor converts a measurement.
 * @details Start a measurement, keep running other code during the sensor conversion, then read the result when it is ready
 * @copyright   Copyright (c) 2026 David J. Schinsing
 * @licence     The MIT License (MIT)
 * @author [David J. Schinsing](https://github.com/dschinsing3)
 * @version  V1.0.1
 * @date  2026-10-26
 * @url https://github.com/DFRobot/DFRobot_LWLP
 */
#include <DFRobot_LWLP.h>

DFRobot_LWLP lwlp;
bool measuring = false;
unsigned long lastAttempt = 0;

void setup() {
  Serial.begin(9600);
  // begin() retains its blocking startup calibration.
  while (lwlp.begin() != ERR_OK) {
    Serial.println("Sensor initialization failed; check wiring.");
    delay(1000);
  }
  lastAttempt = millis() - 500;
}

void loop() {
  if (!measuring && (unsigned long)(millis() - lastAttempt) >= 500) {
    lastAttempt = millis();
    measuring = lwlp.startMeasurement();
    if (!measuring) {
      Serial.println("Measurement command failed.");
    }
  }

  if (measuring && lwlp.dataReady()) {   // this check for dataReady() doesn't block for 30 ms
    DFRobot_LWLP::sLwlp_t data;
    if (lwlp.readData(data)) {
      Serial.print("Temperature: ");
      Serial.print(data.temperature);
      Serial.println(" C");
      Serial.print("Differential Pressure: ");
      Serial.print(data.presure);
      Serial.println(" Pa");
    } else {
      Serial.println("Measurement read failed.");
    }
    measuring = false;
  }

  // Service networking, a web server, or other sensors here.
}
