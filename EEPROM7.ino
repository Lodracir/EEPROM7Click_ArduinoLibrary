#include <SPI.h>
#include "EEPROM7.h"

#define MEMORY_CS_PIN     8
#define MEMORY_WP_PIN     A6
#define MEMORY_HOLD_PIN   6
#define SERIAL_BAUDRATE   115200

//Local variables
uint8_t TXBuffer[3] = {0x34, 0x42, 0x54};
uint8_t RXBuffer[3] = {0};

//Define objects
EEPROM7 Memory(MEMORY_CS_PIN, MEMORY_WP_PIN, MEMORY_HOLD_PIN);

void setup() {
  //Start SPI Serial communication
  Serial.begin(SERIAL_BAUDRATE);
  SPI.begin();

  //Get Memory information
  Memory.GetDeviceInformation();

  //Write Data
  Memory.Write(0x0, (void *)TXBuffer, 3);

  //Read Data
  Memory.Read(0x0, (void *)RXBuffer, 3);

}

void loop() {
  // put your main code here, to run repeatedly:

  Serial.print("Manufacturer ID: ");
  Serial.println( Memory.GetManufacturerID(), HEX );
  Serial.print("Device ID: ");
  Serial.println( Memory.GetDeviceID(), HEX );

  //Transmit read data
  Serial.print("Data 1: 0x");
  Serial.println(RXBuffer[0],HEX);

  Serial.print("Data 2: 0x");
  Serial.println(RXBuffer[1],HEX);

  Serial.print("Data 3: 0x");
  Serial.println(RXBuffer[2],HEX);


  delay(1000);
}
