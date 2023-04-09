#ifndef _EEPROM7_H_
#define _EEPROM7_H_

//Required Includes
#include <Arduino.h>
#include <SPI.h>


class EEPROM7
{
  private:
  //Pins
  uint8_t CS;
  uint8_t WP;
  uint8_t HOLD;

  uint8_t ManufacturerID;
  uint8_t DeviceID;

  //Private functions
  void    ReadDeviceInfo(void);
  uint8_t ReadAddress(uint8_t Address);
  void    EnableWrite(void);
  void    DisableWrite(void);

  public:

  //Constructor
  EEPROM7( uint8_t CS, uint8_t WP, uint8_t HOLD );

  /* ----- Public functions ----- */

  //Read Operation
  void Read(uint32_t Address, void *pData, uint16_t size);

  //Write Operation
  void Write(uint32_t Address, void *pData, uint16_t size);

  //Erase Operation
  void Erase(uint32_t Address, uint16_t size);
  void EraseMemory(void);

  //Memory Information data
  void GetDeviceInformation(void);
  uint8_t GetManufacturerID(void);
  uint8_t GetDeviceID(void);
  

};


#endif // _EEPROM7_H_