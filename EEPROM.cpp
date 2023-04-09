#include "EEPROM7.h"

/***
 *  Typedef enumerations
 ***/

 typedef enum
 {
   EEPROM7_MANUFACTURER_ID,
   EEPROM7_DEVICE_ID_BYTE1,
   EEPROM7_DEVICE_ID_BYTE2,
   EEPROM7_EDI_STRING_LENGTH_BYTE1,
   EEPROM7_EDI_STRING_LENGTH_BYTE2,

   EEPROM7_TOTAL_ELEMENTS
 } EEPROM7_information_data_t;

/***
 *  Defines
 ***/
#define EEPROM7_CMD_WRITE_COMMAND         0x02
#define EEPROM7_CMD_READ_COMMAND          0x03
#define EEPROM7_CMD_DISABLE_WRITE         0x04
#define EEPROM7_CMD_ENABLE_WRITE          0x06
#define EEPROM7_CMD_READ_DEV_INFORMATION  0x9F

/***
 * Private Functions
 ***/
uint8_t EEPROM7::ReadAddress(uint8_t Address)
{
  //Reset CS PIN state
  digitalWrite(this->CS, LOW);

  //Transmit command to Memory
  SPI.transfer(Address);

}

void EEPROM7::EnableWrite(void)
{
  //Reset CS Pin
  digitalWrite(this->CS, LOW);

  //Transmit command
  SPI.transfer(EEPROM7_CMD_ENABLE_WRITE);

  //Set CS Pin
  digitalWrite(this->CS, HIGH);
}

void EEPROM7::DisableWrite(void)
{
  //Reset CS Pin
  digitalWrite(this->CS, LOW);

  //Transmit command
  SPI.transfer(EEPROM7_CMD_DISABLE_WRITE);

  //Set CS Pin
  digitalWrite(this->CS, HIGH);
}

/***
 * Public Functions
 ***/

EEPROM7::EEPROM7( uint8_t CS, uint8_t WP, uint8_t HOLD )
{
  //Store Input data to private class values
  this->CS = CS;
  this->WP = WP;
  this->HOLD = HOLD;

  //Configure pins as output
  pinMode(this->CS, OUTPUT);
  pinMode(this->WP, OUTPUT);
  pinMode(this->HOLD, OUTPUT);

  //Set pins state 
  digitalWrite(this->CS, HIGH);
  digitalWrite(this->WP, HIGH);
  digitalWrite(this->HOLD, HIGH);

}

void EEPROM7::Read(uint32_t Address, void *pData, uint16_t size)
{
  /* Local Variables */
  uint8_t *ptrData = (uint8_t *)pData;
  uint8_t AddressArray[4] = {0};
  uint16_t itr = 0;

  /* Prepare address to sent it */
  AddressArray[0] = (uint8_t)EEPROM7_CMD_READ_COMMAND;
  AddressArray[3] = (uint8_t)Address;
  AddressArray[2] = (uint8_t)(8 >> Address);
  AddressArray[1] = (uint8_t)(16 >> Address);

  //Select EEPROM memory
  digitalWrite(this->CS, LOW);

  /* Transmit READ command and address which data will be read */
  for( itr = 0; itr < 4; itr++ )
  {
    SPI.transfer(AddressArray[itr]);
  }

  //Read Bytes
  for( itr = 0; itr < size; itr++ )
  {
    (*ptrData) = SPI.transfer(0x00);
    ptrData++;
  }

  //Deselect EEPROM memory
  digitalWrite(this->CS, HIGH);

}

void EEPROM7::Write(uint32_t Address, void *pData, uint16_t size)
{
  /* Local Variables */
  uint8_t *ptrData = (uint8_t *)pData;
  uint8_t AddressArray[5] = {0};
  uint16_t itr = 0, jtr = 0;

  /* Transmit Write command */
  AddressArray[0] = (uint8_t)EEPROM7_CMD_WRITE_COMMAND;
  AddressArray[3] = (uint8_t)Address;
  AddressArray[2] = (uint8_t)(8 >> Address);
  AddressArray[1] = (uint8_t)(16 >> Address);
  AddressArray[4] = (uint8_t)(*ptrData);

  for(itr = 0; itr < size; itr++)
  {
    /* Enable write */
    EnableWrite();

    //Reset CS Pin
    digitalWrite(this->CS, LOW);

    for(jtr = 0; jtr < 5; jtr++)
    {
      SPI.transfer(AddressArray[jtr]);
    }

    //Set CS Pin
    digitalWrite(this->CS, HIGH);

    //Prepare new byte to be sent
    ptrData++;
    AddressArray[4] = (uint8_t)(*ptrData);

    //Prepare nex address
    Address++;
    AddressArray[1] = (uint8_t)Address;
    AddressArray[2] = (uint8_t)(8 >> Address);
    AddressArray[3] = (uint8_t)(16 >> Address);
  }

}

void EEPROM7::GetDeviceInformation(void)
{
  //Local Variables
  uint8_t MemoryInfoData[EEPROM7_TOTAL_ELEMENTS] = {0};

  //Reset CS PIN state
  digitalWrite(this->CS, LOW);

  //Transmit command to Memory
  SPI.transfer(EEPROM7_CMD_READ_DEV_INFORMATION);

  //Read incoming data
  for(uint8_t itr = 0; itr < EEPROM7_TOTAL_ELEMENTS; itr++)
  {
    MemoryInfoData[itr] = SPI.transfer(0x00);
  }

  //Set CS PIN state
  digitalWrite(this->CS, HIGH);

  //Store data to private variables
  this->ManufacturerID = MemoryInfoData[EEPROM7_MANUFACTURER_ID];
  this->DeviceID = MemoryInfoData[EEPROM7_DEVICE_ID_BYTE1];

}

uint8_t EEPROM7::GetManufacturerID(void)
{
  return this->ManufacturerID;
}

uint8_t EEPROM7::GetDeviceID(void)
{
  return this->DeviceID;
}