#include <Wire.h>                              // Wire library - used for I2C communication
#include <LiquidCrystal_I2C.h>

#define ADXL345  0x53                          // The ADXL345 sensor I2C address
#define REG_SIZE 0x06                          // 6 registers to read
float X, Y, Z;                                 // Outputs
unsigned long time;
uint8_t incomingByte;
char row1[10];
char row2[2];
char row3[10];
uint8_t currentRow = 1;
int32_t result;

void sendDataToESP();
LiquidCrystal_I2C lcd(0x27, 20, 4);            // Set the LCD address to 0x27 for a 20 chars and 4 line display
void lcdPrint(uint8_t incomingByte);
void rowFill(uint8_t row, char* ch);

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

  lcd.begin();                                 // initialize the LCD
  lcd.backlight();                             // Turn on the blacklight and print a message.
  lcd.setCursor(0,0);                          // sets cursor to column: 0 , row: 0
  lcd.print(atol(row1));                       // print X value to the screen
  time = millis();
}

void loop() {
  sendDataToESP();                             // function used to send data to ESP app
  delay(10);

  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    if (incomingByte >= 48 && incomingByte <= 57) {
      lcdPrint(incomingByte);
    }
  }
}

void sendDataToESP() {
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

void lcdPrint(uint8_t incomingByte) {
  switch(incomingByte) {
  case '1':
    rowFill(currentRow, "0");
    break;
  case '2':
    rowFill(currentRow, "1");
    break;
  case '3':
    rowFill(currentRow, "2");
    break;
  case '4':
    rowFill(currentRow, "3");
    break;
  case '5':
    rowFill(currentRow, "4");
    break;
  case '6':
    rowFill(currentRow, "5");
    break;
  case '7':
    if (currentRow == 1) {
      currentRow = 3;
      strcpy(row2, "*");
    }
    break;
  case '8':
    if (currentRow == 3) {
      result = atol(row1) * atol(row3);
      currentRow = 4;
    }
    break;
  case '9':
      strcpy(row1, "");
      strcpy(row2, "");
      strcpy(row3, "");
      currentRow = 1
    break;
  default:
    break;
  }

  switch(currentRow) {
  case 1:
    lcd.clear();                                 // clear previous values from the screen
    lcd.setCursor(0,0);                          // sets cursor to column: 0 , row: 0
    lcd.print(atol(row1));
    break;
  case 3:
    lcd.clear();                                 // clear previous values from the screen
    lcd.setCursor(0,0);                          // sets cursor to column: 0 , row: 0
    lcd.print(atol(row1));
    lcd.setCursor(0,1);                          // sets cursor to column: 0 , row: 1
    lcd.print(row2);
    lcd.setCursor(0,2);                          // sets cursor to column: 0 , row: 2
    lcd.print(atol(row3));
    break;
  case 4:
    lcd.clear();                                 // clear previous values from the screen
    lcd.setCursor(0,0);                          // sets cursor to column: 0 , row: 0
    lcd.print(atol(row1));
    lcd.setCursor(0,1);                          // sets cursor to column: 0 , row: 1
    lcd.print(row2);
    lcd.setCursor(0,2);                          // sets cursor to column: 0 , row: 2
    lcd.print(atol(row3));
    lcd.setCursor(0,2);                          // sets cursor to column: 0 , row: 2
    lcd.print(result);
    break;
  }
}

void rowFill(uint8_t row, char* ch) {
  switch(row) {
  case 1:
    if (strlen(row1) < 9)
      strcat(row1, ch);
    break;
  case 3:
    if (strlen(row3) < 9)
      strcat(row3, ch);
    break;
  default:
    break;
  }
}
