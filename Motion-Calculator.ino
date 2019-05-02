#include <stdio.h>
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

LiquidCrystal_I2C lcd(0x27, 20, 4);            // Set the LCD address to 0x27 for a 20 chars and 4 line display
void lcdPrint(uint8_t incomingByte);

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
  time = millis();
  Serial.println(0);
}
void loop() {
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    if (incomingByte >= 48 && incomingByte <= 57) {
      lcdPrint(incomingByte);  
    }
  } 
 
  if (millis() - time > 10000) {
    time = millis();
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
    
    lcd.clear();                                 // clear previous values from the screen
    lcd.setCursor(0,0);                          // sets cursor to column: 0 , row: 0
    lcd.print("X:");                             // print X: to the screen
    lcd.setCursor(2,0);                          // sets cursor to column: 0 , row: 0
    lcd.print(X);                                // print X value to the screen
  }
}

void lcdPrint(uint8_t incomingByte) {
  switch(incomingByte) {
  case '1':
    switch(currentRow) {
    case 1:
      if (strlen(row1) < 9)
        strcat(row1, "0");
      break;
    case 3:
      if (strlen(row3) < 9)
        strcat(row3, "0");
      break;
    default:
      break;
    }
    break;
  case '2':
    switch(currentRow) {
    case 1:
      if (strlen(row1) < 9)
        strcat(row1, "1");
      break;
    case 3:
      if (strlen(row3) < 9)
        strcat(row3, "1");
      break;
    default:
      break;
    }
    break;
  case '3':
    switch(currentRow) {
    case 1:
      if (strlen(row1) < 9)
        strcat(row1, "2");
      break;
    case 3:
      if (strlen(row3) < 9)
        strcat(row3, "2");
      break;
    default:
      break;
    }
    break;
  case '4':
    switch(currentRow) {
    case 1:
      if (strlen(row1) < 9)
        strcat(row1, "3");
      break;
    case 3:
      if (strlen(row3) < 9)
        strcat(row3, "3");
      break;
    default:
      break;
    }
    break;
  case '5':
    switch(currentRow) {
    case 1:
      if (strlen(row1) < 9)
        strcat(row1, "4");
      break;
    case 3:
      if (strlen(row3) < 9)
        strcat(row3, "4");
      break;
    default:
      break;
    }
    break;
  case '6':
    switch(currentRow) {
    case 1:
      if (strlen(row1) < 9)
        strcat(row1, "5");
      break;
    case 3:
      if (strlen(row3) < 9)
        strcat(row3, "5");
      break;
    default:
      break;
    }
    break;
  case '7':
    if (currentRow == 1) {
      currentRow = 3;    
      strcpy(row2, "+");
    }
    break;
  case '8':
    if (currentRow == 1) {
      currentRow = 3;    
      strcpy(row2, "-");
    }
    break;  
  case '9':
    if (currentRow == 1 || currentRow == 4) {
      Serial.println("Clear screen");
      strcpy(row1, "");
      strcpy(row2, "");
      strcpy(row3, "");
      currentRow = 1;
      Serial.println(atol(row1));
      Serial.println(row2);
      Serial.println(atol(row3));
      Serial.println("Cleared screen");
    } else if (currentRow == 3) {
      if (strcmp(row2, "-")) {
        result = atol(row1) + atol(row3);
      } else {
        result = atol(row1) - atol(row3);
      }
      Serial.println(atol(row1));
      Serial.println(row2);
      Serial.println(atol(row3));
      Serial.print("Result is: "); 
      Serial.println(String(result)); 
      currentRow = 4;    
    }    
    break;
  default:
    break;
  }

  switch(currentRow) {
  case 1:
    Serial.println(atol(row1));
    break;
  case 3:
    Serial.println(atol(row1));
    Serial.println(row2);
    Serial.println(atol(row3));
    break;
  }
}
