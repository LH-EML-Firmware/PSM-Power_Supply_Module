#ifndef NVM_CONFIG_H
#define NVM_CONFIG_H

#include "mcc_generated_files/system/system.h"
#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

// Base address for your configuration data is EEPROM_START_ADDRESS
// The size is EEPROM_SIZE          (256U)
#define EEPROM_CONFIG_MAGIC         0xA5

// EEPROM addresses for holding registers
#define EEPROM_MAGIC_ADDR           (EEPROM_START_ADDRESS + 0)      // Check whether the memory has already been written or not
#define EEPROM_ADDR_SLAVE_ADDR      (EEPROM_START_ADDRESS + 1)      // Holding Register 0
#define EEPROM_BAUDRATE_ADDR        (EEPROM_START_ADDRESS + 3)      // Holding Register 1
#define EEPROM_PERIODE_ADDR         (EEPROM_START_ADDRESS + 5)      // Holding Register 2
#define EEPROM_VOLTAGE_CHRG_ON_ADDR (EEPROM_START_ADDRESS + 7)      // Holding Register 3
#define EEPROM_CURR_TAIL_ADDR       (EEPROM_START_ADDRESS + 9)      // Holding Register 4
#define EEPROM_BEACON_ADDR          (EEPROM_START_ADDRESS + 11)     // Holding Register 5
#define EEPROM_BEACON_MODE_ADDR     (EEPROM_START_ADDRESS + 13)     // Holding Register 6
#define EEPROM_BEACON_DUTY_ADDR     (EEPROM_START_ADDRESS + 15)     // Holding Register 7
#define EEPROM_UVP_ADDR             (EEPROM_START_ADDRESS + 17)     // Holding Register 8
#define EEPROM_UVP_MODE_ADDR        (EEPROM_START_ADDRESS + 19)     // Holding Register 9
#define EEPROM_CHRG_ADDR            (EEPROM_START_ADDRESS + 21)     // Holding Register 10
#define EEPROM_CHRG_MODE_ADDR       (EEPROM_START_ADDRESS + 23)     // Holding Register 11

#define SENSOR_TYPE_ADDR            (EEPROM_START_ADDRESS + 25)      // Input Register 0
#define SERIAL_NUMBER_ADDR          (EEPROM_START_ADDRESS + 27)      // Input Register 1
#define CHRG_STATUS_ADDR            (EEPROM_START_ADDRESS + 29)      // Input Register 2

// Function prototype
uint16_t EEPROM_ReadWord(uint16_t address);
bool EEPROM_WriteWord(uint16_t address, uint16_t value);

#endif
