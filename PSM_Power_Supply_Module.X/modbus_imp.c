/*
 * File:   modbus_imp.c
 * Author: asus
 *
 * Created on 24 de agosto de 2023, 12:00
 */

/*
   This example application sets up an RTU server and handles modbus requests

   This server supports the following function codes:
   FC 01 (0x01) Read Coils
   FC 03 (0x03) Read Holding Registers
   FC 15 (0x0F) Write Multiple Coils
   FC 16 (0x10) Write Multiple registers
*/

#include "mcc_generated_files/system/system.h"
#include <xc.h>
#include "modbus_imp.h"
#include "nvm_config.h"
#include "eusart1_utils.h"

int32_t read_serial(uint8_t* buf, uint16_t count, int32_t byte_timeout_ms, void* arg) 
{    
    int32_t charCount = 0;
    uint32_t timeout = 50000; // Adjust as needed

    while (charCount < count) {
        uint32_t t = 0;
        while (!EUSART1_IsRxReady()) {
            if (t++ > timeout) return charCount > 0 ? charCount : NMBS_ERROR_TIMEOUT;
        }
        buf[charCount++] = EUSART1_Read();
    }

    return charCount;
}

int32_t write_serial(const uint8_t* buf, uint16_t count, int32_t byte_timeout_ms, void* arg)
{ 
    RW_SetHigh();  // Enable TX  // Use for 485 driver
    int32_t number_of_byte_send = 0;

    while(number_of_byte_send<count)
    {
        if(EUSART1_IsTxReady())
        {
            EUSART1_Write(*buf);
            while(!EUSART1_IsTxDone());
            buf++;
            number_of_byte_send++;
        }
    }
    while(!EUSART1_IsTxDone());
    RW_SetLow(); // Disable TX  // Use for 485 driver
    
    return number_of_byte_send;
}


nmbs_error handle_read_coils(uint16_t address, uint16_t quantity, nmbs_bitfield coils_out, uint8_t unit_id, void *arg) {
  if (address + quantity > COILS_ADDR_MAX)
    return NMBS_EXCEPTION_ILLEGAL_DATA_ADDRESS;

  nmbs_bitfield *server_coils = &((mod_bus_registers*) arg)->server_coils.coils;
  // Read our coils values into coils_out
  for (int i = 0; i < quantity; i++) {
    bool value = nmbs_bitfield_read(*server_coils, address + i);
    nmbs_bitfield_write(coils_out, i, value);
  }

  return NMBS_ERROR_NONE;
}


nmbs_error handle_write_single_coil(uint16_t address, bool coils, uint8_t unit_id, void *arg) {
  if (address > COILS_ADDR_MAX)
    return NMBS_EXCEPTION_ILLEGAL_DATA_ADDRESS;

  // Write coils values to our server_coils
  nmbs_bitfield *server_coils = &((mod_bus_registers*) arg)->server_coils.coils;
  nmbs_bitfield_write(*server_coils, address, coils);
  
  return NMBS_ERROR_NONE;
}

nmbs_error handler_read_input_registers(uint16_t address, uint16_t quantity, uint16_t* registers_out, uint8_t unit_id, void *arg) {
    if (address + quantity > REGS_INPUT_ADDR_MAX)
        return NMBS_EXCEPTION_ILLEGAL_DATA_ADDRESS;

    uint16_t* server_registers = (uint16_t *)(&(((mod_bus_registers*) arg)->server_input_register));
    for (int i = 0; i < quantity; i++)
        registers_out[i] = server_registers[address + i];

    return NMBS_ERROR_NONE;
}

nmbs_error handler_read_holding_registers(uint16_t address, uint16_t quantity, uint16_t* registers_out, uint8_t unit_id, void *arg) {
  if (address + quantity > REGS_HOLDING_ADDR_MAX)
    return NMBS_EXCEPTION_ILLEGAL_DATA_ADDRESS;

  // Read our registers values into registers_out
  uint16_t* server_registers = (uint16_t *)(&(((mod_bus_registers*) arg)->server_holding_register));
  for (int i = 0; i < quantity; i++)
    registers_out[i] = server_registers[address + i];

  return NMBS_ERROR_NONE;
}


nmbs_error handle_write_single_register(uint16_t address, const uint16_t* registers, uint8_t unit_id, void *arg) {
  if (address > REGS_HOLDING_ADDR_MAX)
    return NMBS_EXCEPTION_ILLEGAL_DATA_ADDRESS;

  // Write registers values to our server_registers
  uint16_t* server_registers = (uint16_t *)(&(((mod_bus_registers*) arg)->server_holding_register));

  server_registers[address] = registers;

  return NMBS_ERROR_NONE;
}

void set_holding_regs_to_default(holding_register* regs)
{
    regs->addr_slave        = RTU_SERVER_ADDRESS_DEFAULT;
    regs->baudrate          = RTU_BAUDRATE_DEFAULT;
    regs->periode           = 100;
    regs->voltage_chrg_on   = VOLTAGE_CHRG_ON;
    regs->curr_tail         = CURR_TAIL;
    regs->beacon            = 0;
}

void default_values_register(mod_bus_registers* registers)
{
    m_memset(&(registers->server_coils),            0 ,sizeof(registers->server_coils));
    m_memset(&(registers->server_input_register),   0 ,sizeof(registers->server_input_register));
    m_memset(&(registers->server_holding_register), 0 ,sizeof(registers->server_holding_register));
        
    registers->server_holding_register.periode          = 100;
    registers->server_holding_register.voltage_chrg_on  = VOLTAGE_CHRG_ON;
    registers->server_holding_register.curr_tail        = CURR_TAIL;
    registers->server_holding_register.beacon           = 1;
    
    registers->server_input_register.serial_number      = RTU_SERIAL_NUMBER_DEFAULT;
    registers->server_input_register.sensor_type        = RTU_SENSOR_TYPE_DEFAULT;
    
    // Slave Number and Baudrate could have been stored in the Nov Volatile Memory
    // The first time the NVM is written we write NVM_CONFIG_MAGIC in the first address to indicate that the NVM contains usable data.
    while (NVM_IsBusy());   // Wait until the NVM is ready before reding
    if(EEPROM_Read(EEPROM_MAGIC_ADDR) != EEPROM_CONFIG_MAGIC)
    {
        registers->server_holding_register.addr_slave       = RTU_SERVER_ADDRESS_DEFAULT;
        registers->server_holding_register.baudrate         = RTU_BAUDRATE_DEFAULT;
        registers->server_input_register.sensor_type        = RTU_SENSOR_TYPE_DEFAULT;
        registers->server_input_register.serial_number      = RTU_SERIAL_NUMBER_DEFAULT;
        
        // Indicate that the EEPROM NVM Memory has been written at least once from this point on by writing the magic config value into the first address
        NVM_UnlockKeySet(UNLOCK_KEY); 
        while (NVM_IsBusy()); 
        EEPROM_Write(EEPROM_MAGIC_ADDR, EEPROM_CONFIG_MAGIC);
        
        // Write default values for holding registers into memory
        EEPROM_WriteWord(EEPROM_ADDR_SLAVE_ADDR, RTU_SERVER_ADDRESS_DEFAULT);
        EEPROM_WriteWord(EEPROM_BAUDRATE_ADDR, RTU_BAUDRATE_DEFAULT);
        EEPROM_WriteWord(EEPROM_PERIODE_ADDR, 100);
        EEPROM_WriteWord(EEPROM_VOLTAGE_CHRG_ON_ADDR, VOLTAGE_CHRG_ON);
        EEPROM_WriteWord(EEPROM_CURR_TAIL_ADDR, CURR_TAIL);
        EEPROM_WriteWord(EEPROM_BEACON_ADDR, 1);
        
        // Write default values for input registers into memory
        EEPROM_WriteWord(SENSOR_TYPE_ADDR, RTU_SENSOR_TYPE_DEFAULT);
        EEPROM_WriteWord(SERIAL_NUMBER_ADDR, RTU_SERIAL_NUMBER_DEFAULT);
    }
    else
    {
        // Load Input Register initial values from NVM
        registers->server_holding_register.addr_slave       = EEPROM_ReadWord(EEPROM_ADDR_SLAVE_ADDR);
        registers->server_holding_register.baudrate         = EEPROM_ReadWord(EEPROM_BAUDRATE_ADDR);
        registers->server_holding_register.periode          = EEPROM_ReadWord(EEPROM_PERIODE_ADDR);
        registers->server_holding_register.voltage_chrg_on  = EEPROM_ReadWord(EEPROM_VOLTAGE_CHRG_ON_ADDR);
        registers->server_holding_register.curr_tail        = EEPROM_ReadWord(EEPROM_CURR_TAIL_ADDR);
        registers->server_holding_register.beacon           = EEPROM_ReadWord(EEPROM_BEACON_ADDR);
        
        // Apply configurations contained in holding registers
        EUSART1_SetBaudRate(registers->server_holding_register.baudrate);
        
        // Load Input Register initial values from NVM
        registers->server_input_register.sensor_type      = EEPROM_ReadWord(SENSOR_TYPE_ADDR);
        registers->server_input_register.serial_number    = EEPROM_ReadWord(SERIAL_NUMBER_ADDR);

    }
}

void holding_register_change_handler(mod_bus_registers* modbus_data,holding_register* prev_holding_regs, nmbs_t* nmbs) // nmbs_t* nmbs 
{    
    // Check for Salve Num (RTU Address) change
    if(modbus_data->server_holding_register.addr_slave != prev_holding_regs->addr_slave)
    {
        prev_holding_regs->addr_slave = modbus_data->server_holding_register.addr_slave;                // Update previous holding register value
        EEPROM_WriteWord(EEPROM_ADDR_SLAVE_ADDR, modbus_data->server_holding_register.addr_slave);      // Update non volatile memory  
        nmbs->address_rtu = (uint8_t)modbus_data->server_holding_register.addr_slave;                   // Update modbus server slave address
    }
    
    // Check for baudrate changes 
    if(modbus_data->server_holding_register.baudrate != prev_holding_regs->baudrate)
    {
        if(EUSART1_SetBaudRate(modbus_data->server_holding_register.baudrate))              // Set UART Baudrate to the new value. This returns true on success
        {
            prev_holding_regs->baudrate = modbus_data->server_holding_register.baudrate;    // Update shadow copy with the new value
            EEPROM_WriteWord(EEPROM_BAUDRATE_ADDR, modbus_data->server_holding_register.baudrate);
        }
        else
        {
            modbus_data->server_holding_register.baudrate = prev_holding_regs->baudrate; // Baudrate invalid, revert to previous
        }
    }
    
    // Check for changes in measuring periode
    if(modbus_data->server_holding_register.periode != prev_holding_regs->periode)
    {
        prev_holding_regs->periode = modbus_data->server_holding_register.periode;
        EEPROM_WriteWord(EEPROM_PERIODE_ADDR, modbus_data->server_holding_register.periode);
    }
    
    // Check for changes in reset voltage
    if(modbus_data->server_holding_register.voltage_chrg_on != prev_holding_regs->voltage_chrg_on)
    {
        prev_holding_regs->voltage_chrg_on = modbus_data->server_holding_register.voltage_chrg_on;
        EEPROM_WriteWord(EEPROM_VOLTAGE_CHRG_ON_ADDR, modbus_data->server_holding_register.voltage_chrg_on);
    }
    
    // Check for changes in tail current
    if(modbus_data->server_holding_register.curr_tail != prev_holding_regs->curr_tail)
    {
        prev_holding_regs->curr_tail = modbus_data->server_holding_register.curr_tail;
        EEPROM_WriteWord(EEPROM_CURR_TAIL_ADDR, modbus_data->server_holding_register.curr_tail);
    }
    
    // Check for changes in beacon ON/OFF
    if(modbus_data->server_holding_register.beacon != prev_holding_regs->beacon)
    {
        prev_holding_regs->beacon = modbus_data->server_holding_register.beacon;
                EEPROM_WriteWord(EEPROM_BEACON_ADDR, modbus_data->server_holding_register.beacon);
    }
}

void single_16_bit_nvm_write(uint16_t value)
{
    /*flash_address_t base = NVM_CONFIG_BASE_ADDR;
    flash_data_t flash_row[PROGMEM_PAGE_SIZE] = {0};  // Ensure size matches page size (e.g., 32)
    
    // Step 1: Read current flash content into flash_row
    for (uint8_t i = 0; i < PROGMEM_PAGE_SIZE; i++) 
    {
        flash_row[i] = FLASH_Read(base + i);
    }
    
    // Step 2: Modify only the changed value
    flash_row[NVM_ADDR_SLAVE_OFFSET]*/ 
    
}

void check_error_modbus(nmbs_error err)
{
//    switch(err)
//    {
//        MBS_ERROR_INVALID_UNIT_ID = -7,  /**< Received invalid unit ID in response from server */
//        NMBS_ERROR_INVALID_TCP_MBAP = -6, /**< Received invalid TCP MBAP */
//        NMBS_ERROR_CRC = -5,              /**< Received invalid CRC */
//        NMBS_ERROR_TRANSPORT = -4,        /**< Transport error */
//        NMBS_ERROR_TIMEOUT = -3,          /**< Read/write timeout occurred */
//        NMBS_ERROR_INVALID_RESPONSE = -2, /**< Received invalid response from server */
//        NMBS_ERROR_INVALID_ARGUMENT = -1, /**< Invalid argument provided */
//        NMBS_ERROR_NONE = 0,              /**< No error */
//
//        // Modbus exceptions
//        NMBS_EXCEPTION_ILLEGAL_FUNCTION = 1,      /**< Modbus exception 1 */
//        NMBS_EXCEPTION_ILLEGAL_DATA_ADDRESS = 2,  /**< Modbus exception 2 */
//        NMBS_EXCEPTION_ILLEGAL_DATA_VALUE = 3,    /**< Modbus exception 3 */
//        NMBS_EXCEPTION_SERVER_DEVICE_FAILURE = 4, /**< Modbus exception 4 */
//        case NMBS_ERROR_CRC:
//            //sendPacket((char*)&sensor.sensor_info, sizeof(sensor_info_t));
//            sendSensorInfo(&sensor.sensor_info);
//        case CMD_SENSE:
//            sensor_data = sensor.sense(&sensor.sensor_info.ADCChannel);
//            break;
//        case CMD_GET_DATA:
//            sendSensorData(sensor_data);
//            break;
//        default:
//            //sendPacket("INVALIDO", 8);
//            break;
//       }
}

