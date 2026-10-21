/*!
 * @file readDataNonBlocking.ino
 * @brief Keep loop responsive while the sensor converts a measurement.
 * @license The MIT License (MIT)
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

  if (measuring && lwlp.dataReady()) {
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
