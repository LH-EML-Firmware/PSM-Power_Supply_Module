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
#include <stdbool.h>
#include "parameters.h"
#include "modbus_imp.h"
#include "nvm_config.h"
#include "eusart1_utils.h"

float upscaled_von = 0.0;
float aux_result = 0.0;

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

  // Read our registers values into registers_out
  uint16_t* server_registers = (uint16_t *)(&(((mod_bus_registers*) arg)->server_input_register));
  uint16_t* panel_registers = (uint16_t *)((((mod_bus_registers*) arg)->server_input_register.panel_data));
  uint16_t* bat_registers = (uint16_t *)((((mod_bus_registers*) arg)->server_input_register.battery_data));
  uint16_t* cons_registers = (uint16_t *)((((mod_bus_registers*) arg)->server_input_register.cons_data));

  for (uint8_t i = 0; i < quantity; i++)
  {
      if(address + i >=0 && address + i <PANEL_HIST_VOLT_ADDR)
      {
        registers_out[i] = server_registers[address + i];
      }
      else if(address + i >=PANEL_HIST_VOLT_ADDR && address + i <BAT_HIST_VOLT_ADDR)
      {
        registers_out[i] = panel_registers[address + i - PANEL_HIST_VOLT_ADDR];
      }
      else if(address + i >=BAT_HIST_VOLT_ADDR && address + i <CONS_HIST_VOLT_ADDR)
      {
        registers_out[i] = bat_registers[address + i - BAT_HIST_VOLT_ADDR];
      }
      else
      {
        registers_out[i] = cons_registers[address + i - CONS_HIST_VOLT_ADDR];
      }
  }
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
    regs->addr_slave                = RTU_SERVER_ADDRESS_DEFAULT;
    regs->baudrate                  = RTU_BAUDRATE_DEFAULT;
    
    regs->periode                   = TMR0_PeriodGet();
    regs->voltage_chrg_on           = VOLTAGE_CHRG_ON;
    regs->curr_tail                 = CURR_TAIL;
    regs->beacon                    = BEACONS_OFF;
    regs->beacon_mode               = BEACONS_MODE_AUTO;
    regs->beacon_duty_mode          = BEACONS_DUTY_BLINK;
    regs->uvp                       = 1;
    regs->uvp_mode                  = UVP_MODE_AUTO;
    regs->chrg                      = RTU_CHRG_STATUS_DEFAULT;
    regs->chrg_mode                 = CHRG_MODE_AUTO;
    
    regs->panel_volt_calib_factor   = DEFAULT_PANEL_VOLT_CALIB_FACTOR;
    regs->batt_volt_calib_factor    = DEFAULT_BATT_VOLT_CAL_FACTOR;
    regs->cons_volt_calib_factor    = DEFAULT_CONS_VOLT_CAL_FACTOR;
    
    regs->serial_number_in          = 0;
    regs->sn_password               = 0;
    regs->sn_write_status           = SNW_STATUS_IDLE;
    
    regs->panel_curr_calib_factor   = DEFAULT_PANEL_CURR_CALIB_FACTOR;
    regs->batt_curr_calib_factor    = DEFAULT_BATT_CURR_CALIB_FACTOR;
    regs->cons_curr_calib_factor    = DEFAULT_CONS_CURR_CALIB_FACTOR;
}

void default_values_register(mod_bus_registers* registers)
{
    m_memset(&(registers->server_coils),            0 ,sizeof(registers->server_coils));
    m_memset(&(registers->server_input_register),   0 ,sizeof(registers->server_input_register));
    m_memset(&(registers->server_holding_register), 0 ,sizeof(registers->server_holding_register));
        
    registers->server_holding_register.periode                      = TMR0_PeriodToMilliseconds(TMR0_PeriodGet());
    registers->server_holding_register.voltage_chrg_on              = VOLTAGE_CHRG_ON;
    registers->server_holding_register.curr_tail                    = CURR_TAIL;
    registers->server_holding_register.beacon                       = BEACONS_OFF;
    registers->server_holding_register.beacon_mode                  = BEACONS_MODE_AUTO;
    registers->server_holding_register.beacon_duty_mode             = BEACONS_DUTY_BLINK;
    registers->server_holding_register.uvp                          = 1;
    registers->server_holding_register.uvp_mode                     = UVP_MODE_AUTO;
    registers->server_holding_register.chrg                         = RTU_CHRG_STATUS_DEFAULT;
    registers->server_holding_register.chrg_mode                    = CHRG_MODE_AUTO;
    
    registers->server_holding_register.panel_volt_calib_factor      = DEFAULT_PANEL_VOLT_CALIB_FACTOR;
    registers->server_holding_register.batt_volt_calib_factor       = DEFAULT_BATT_VOLT_CAL_FACTOR;
    registers->server_holding_register.cons_volt_calib_factor       = DEFAULT_CONS_VOLT_CAL_FACTOR;
    
    registers->server_holding_register.serial_number_in             = 0;
    registers->server_holding_register.sn_password                  = 0;
    registers->server_holding_register.sn_write_status              = SNW_STATUS_IDLE;
    
    registers->server_holding_register.panel_curr_calib_factor      = DEFAULT_PANEL_CURR_CALIB_FACTOR;
    registers->server_holding_register.batt_curr_calib_factor       = DEFAULT_BATT_CURR_CALIB_FACTOR;
    registers->server_holding_register.cons_curr_calib_factor       = DEFAULT_CONS_CURR_CALIB_FACTOR;
    
    registers->server_input_register.serial_number                  = RTU_SERIAL_NUMBER_DEFAULT;
    registers->server_input_register.sensor_type                    = RTU_SENSOR_TYPE_DEFAULT;
    registers->server_input_register.chrg                           = RTU_CHRG_STATUS_DEFAULT;
    
    // Slave Number and Baudrate could have been stored in the Nov Volatile Memory
    // The first time the NVM is written we write NVM_CONFIG_MAGIC in the first address to indicate that the NVM contains usable data.
    while (NVM_IsBusy());   // Wait until the NVM is ready before reding
    if(EEPROM_Read(EEPROM_MAGIC_ADDR) != EEPROM_CONFIG_MAGIC)
    {
        registers->server_holding_register.addr_slave       = RTU_SERVER_ADDRESS_DEFAULT;
        registers->server_holding_register.baudrate         = RTU_BAUDRATE_DEFAULT;
        registers->server_input_register.sensor_type        = RTU_SENSOR_TYPE_DEFAULT;
        registers->server_input_register.serial_number      = RTU_SERIAL_NUMBER_DEFAULT;
        registers->server_input_register.chrg               = RTU_CHRG_STATUS_DEFAULT;
        
        // Indicate that the EEPROM NVM Memory has been written at least once from this point on by writing the magic config value into the first address
        NVM_UnlockKeySet(UNLOCK_KEY); 
        while (NVM_IsBusy()); 
        EEPROM_Write(EEPROM_MAGIC_ADDR, EEPROM_CONFIG_MAGIC);
        
        // Write default values for holding registers into memory
        EEPROM_WriteWord(EEPROM_ADDR_SLAVE_ADDR, RTU_SERVER_ADDRESS_DEFAULT);
        EEPROM_WriteWord(EEPROM_BAUDRATE_ADDR, RTU_BAUDRATE_DEFAULT);
        EEPROM_WriteWord(EEPROM_PERIODE_ADDR, TMR0_PeriodToMilliseconds(TMR0_PeriodGet()));
        EEPROM_WriteWord(EEPROM_VOLTAGE_CHRG_ON_ADDR, VOLTAGE_CHRG_ON);
        EEPROM_WriteWord(EEPROM_CURR_TAIL_ADDR, CURR_TAIL);
        EEPROM_WriteWord(EEPROM_BEACON_ADDR, BEACONS_OFF);
        EEPROM_WriteWord(EEPROM_BEACON_MODE_ADDR, BEACONS_MODE_AUTO);
        EEPROM_WriteWord(EEPROM_BEACON_DUTY_ADDR, BEACONS_DUTY_BLINK);
        EEPROM_WriteWord(EEPROM_UVP_ADDR, 1);
        EEPROM_WriteWord(EEPROM_UVP_MODE_ADDR, UVP_MODE_AUTO);
        EEPROM_WriteWord(EEPROM_CHRG_ADDR, RTU_CHRG_STATUS_DEFAULT);
        EEPROM_WriteWord(EEPROM_CHRG_MODE_ADDR, CHRG_MODE_AUTO);
        
        // Write default values for input registers into memory
        EEPROM_WriteWord(SENSOR_TYPE_ADDR, RTU_SENSOR_TYPE_DEFAULT);
        EEPROM_WriteWord(SERIAL_NUMBER_ADDR, RTU_SERIAL_NUMBER_DEFAULT);
        EEPROM_WriteWord(CHRG_STATUS_ADDR, RTU_CHRG_STATUS_DEFAULT);
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
        registers->server_holding_register.beacon_mode      = EEPROM_ReadWord(EEPROM_BEACON_MODE_ADDR);
        registers->server_holding_register.beacon_duty_mode = EEPROM_ReadWord(EEPROM_BEACON_DUTY_ADDR);
        registers->server_holding_register.uvp              = EEPROM_ReadWord(EEPROM_UVP_ADDR);
        registers->server_holding_register.uvp_mode         = EEPROM_ReadWord(EEPROM_UVP_MODE_ADDR);
        registers->server_holding_register.chrg             = EEPROM_ReadWord(EEPROM_CHRG_ADDR);
        registers->server_holding_register.chrg_mode        = EEPROM_ReadWord(EEPROM_CHRG_MODE_ADDR);
        
        // Apply configurations contained in holding registers
        EUSART1_SetBaudRate(registers->server_holding_register.baudrate);
        
        // Load Input Register initial values from NVM
        registers->server_input_register.sensor_type        = EEPROM_ReadWord(SENSOR_TYPE_ADDR);
        registers->server_input_register.serial_number      = EEPROM_ReadWord(SERIAL_NUMBER_ADDR);
        registers->server_input_register.chrg               = EEPROM_ReadWord(CHRG_STATUS_ADDR);
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
        TMR0_PeriodSet(TMR0_MillisecondsToPeriod(modbus_data->server_holding_register.periode));
    }
    
    // Check for changes in reset voltage
    if(modbus_data->server_holding_register.voltage_chrg_on != prev_holding_regs->voltage_chrg_on)
    { 
        // The desired Re-enable Voltage will be inputed in Voltsx100 (example for 12.5V write 1250), then converted to it´s ADC equivalent and re-written into the register
        modbus_data->server_holding_register.voltage_chrg_on = 
        (uint16_t)((modbus_data->server_holding_register.voltage_chrg_on * 1000UL + modbus_data->server_holding_register.batt_volt_calib_factor / 2) /
                   modbus_data->server_holding_register.batt_volt_calib_factor);
        prev_holding_regs->voltage_chrg_on = modbus_data->server_holding_register.voltage_chrg_on;
        EEPROM_WriteWord(EEPROM_VOLTAGE_CHRG_ON_ADDR, modbus_data->server_holding_register.voltage_chrg_on);
    }
    
    // Check for changes in tail current
    if(modbus_data->server_holding_register.curr_tail != prev_holding_regs->curr_tail)
    {
        // The desired Tail Current will be inputed in mA, then converted to it´s ADC equivalent and re-written into the register
        modbus_data->server_holding_register.curr_tail = 
        (uint16_t)((modbus_data->server_holding_register.curr_tail * 1000UL + modbus_data->server_holding_register.batt_curr_calib_factor / 2) /
                   modbus_data->server_holding_register.batt_curr_calib_factor);
        prev_holding_regs->curr_tail = modbus_data->server_holding_register.curr_tail;
        EEPROM_WriteWord(EEPROM_CURR_TAIL_ADDR, modbus_data->server_holding_register.curr_tail);
    }
    
    // Check for changes in beacon ON/OFF
    if(modbus_data->server_holding_register.beacon != prev_holding_regs->beacon)
    {
        prev_holding_regs->beacon = modbus_data->server_holding_register.beacon;
        EEPROM_WriteWord(EEPROM_BEACON_ADDR, modbus_data->server_holding_register.beacon);
    }
    
    // Check for changes in beacon mode (MANUAL/AUTO))
    if(modbus_data->server_holding_register.beacon_mode != prev_holding_regs->beacon_mode)
    {
        prev_holding_regs->beacon_mode = modbus_data->server_holding_register.beacon_mode;
        EEPROM_WriteWord(EEPROM_BEACON_MODE_ADDR, modbus_data->server_holding_register.beacon_mode);
    }
    
    // Check for changes in beacons duty mode
    if(modbus_data->server_holding_register.beacon_duty_mode != prev_holding_regs->beacon_duty_mode)
    {
        prev_holding_regs->beacon_duty_mode = modbus_data->server_holding_register.beacon_duty_mode;
        EEPROM_WriteWord(EEPROM_BEACON_DUTY_ADDR, modbus_data->server_holding_register.beacon_duty_mode); 
    }
    
    // Check for changes in uvp
    if(modbus_data->server_holding_register.uvp != prev_holding_regs->uvp)
    {
        prev_holding_regs->uvp = modbus_data->server_holding_register.uvp;
        EEPROM_WriteWord(EEPROM_UVP_ADDR, modbus_data->server_holding_register.uvp);
    }
    
    // Check for changes in uvp mode
    if(modbus_data->server_holding_register.uvp_mode != prev_holding_regs->uvp_mode)
    {
        prev_holding_regs->uvp_mode = modbus_data->server_holding_register.uvp_mode;
        EEPROM_WriteWord(EEPROM_UVP_MODE_ADDR, modbus_data->server_holding_register.uvp_mode);
    }
    
    // Check for changes in uvp
    if(modbus_data->server_holding_register.chrg != prev_holding_regs->chrg)
    {
        prev_holding_regs->chrg = modbus_data->server_holding_register.chrg;
        EEPROM_WriteWord(EEPROM_CHRG_ADDR, modbus_data->server_holding_register.chrg);
    }
    
    // Check whether the correct password has ben submitted in orde to enable a serial number write operation
    if(modbus_data->server_holding_register.sn_password != prev_holding_regs->sn_password)
    {
        //prev_holding_regs->chrg_mode = modbus_data->server_holding_register.chrg_mode;
        //EEPROM_WriteWord(EEPROM_CHRG_MODE_ADDR, modbus_data->server_holding_register.chrg_mode);
        if(!sn_write_enabled)
        {
            if(modbus_data->server_holding_register.sn_password == SN_PASSWORD_CORRECT)
            {
                sn_write_enabled = true;
            }
            else
            {
                modbus_data->server_holding_register.sn_write_status = SNW_STATUS_WRONG_PASS;
                sn_write_enabled = false;
            }
        }
        modbus_data->server_holding_register.sn_password = 0;
        prev_holding_regs->sn_password = 0;
    }
    
    // Check whether a serial number was write was attempted and handle it
    if(modbus_data->server_holding_register.serial_number_in != prev_holding_regs->serial_number_in)
    {
        if((!sn_write_enabled) && (modbus_data->server_holding_register.sn_write_status != SNW_STATUS_WRONG_PASS))
        {
            modbus_data->server_holding_register.sn_write_status = SNW_STATUS_NOT_AUTHORIZED;
        }
        if(sn_write_enabled)
        {
            modbus_data->server_input_register.serial_number = modbus_data->server_holding_register.serial_number_in;
            modbus_data->server_holding_register.sn_write_status = SNW_STATUS_SUCCESS;
            sn_write_enabled = false;
            EEPROM_WriteWord(SERIAL_NUMBER_ADDR, modbus_data->server_input_register.serial_number);
        }
        modbus_data->server_holding_register.serial_number_in = 0;
        prev_holding_regs->serial_number_in = 0;
    }
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

uint16_t TMR0_MillisecondsToPeriod(uint16_t ms)
{
    uint32_t ticks = (uint32_t)ms * 1000UL / TMR0_TICK_US;

    if (ticks >= 65536)
        return 0;  // Minimum period value (max delay)

    return (uint16_t)(65536 - ticks);
}

uint16_t TMR0_PeriodToMilliseconds(uint16_t period)
{
    uint32_t ticks = 65536UL - period;
    return (uint16_t)((ticks * TMR0_TICK_US) / 1000UL);
}
