#include <Wire.h>                              // Wire library - used for I2C communication

#define ADXL345  0x53                          // The ADXL345 sensor I2C address
#define REG_SIZE 0x06                          // 6 registers to read
float X, Y, Z;                                 // Outputs

void setup() {
  Serial.begin(115200);                        // Initiate serial communication for printing the results on the Serial monitor
  Wire.begin();                                // Initiate the Wire library
  // Set ADXL345 in measuring mode
  Wire.beginTransmission(ADXL345);             // Start communicating with the device 
  Wire.write(0x2D);                            // Access/ talk to POWER_CTL Register - 0x2D
  // Enable measurement
  Wire.write(8);                               // (8dec -> 0000 1000 binary) Bit D3 High for measuring enable 
  Wire.endTransmission();
  delay(10);
}
void loop() {
  // === Read acceleromter data === //
  Wire.beginTransmission(ADXL345);
  Wire.write(0x32);                            // Start with register 0x32 (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(ADXL345, REG_SIZE, true);   // Read 6 registers total, each axis value is stored in 2 registers
  X = ( Wire.read()| Wire.read() << 8);        // X-axis value
  X = X/256;                                   //For a range of +-2g, we need to divide the raw values by 256, according to the datasheet
  Y = ( Wire.read()| Wire.read() << 8);        // Y-axis value
  Y = Y/256;
  Z = ( Wire.read()| Wire.read() << 8);        // Z-axis value
  Z = Z/256;
  
  Serial.print(X);
  Serial.print("\t");
  Serial.print(Y);
  Serial.print("\t");
  Serial.println(Z);
}
