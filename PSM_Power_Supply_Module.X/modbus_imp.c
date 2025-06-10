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

int32_t read_serial(uint8_t* buf, uint16_t count, int32_t byte_timeout_ms, void* arg) {
    
    int32_t charCount = 0;
    uint8_t c;
    
    while(charCount != count)
    {
            c = EUSART1_Read();
            buf[charCount++] = c;
    }
//    EUSART1_Write(*buf);
//    while(!EUSART1_is_tx_done());
    return charCount;
}



int32_t write_serial(const uint8_t* buf, uint16_t count, int32_t byte_timeout_ms, void* arg)
{ 
    //TxDeviceEnable();
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
    //TxDeviceDisable();
    
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

void default_values_register(mod_bus_registers* registers)
{
    registers->server_holding_register.addr_slave    =   RTU_SERVER_ADDRESS_DEFAULT;
    registers->server_holding_register.baudrate      =   RTU_BAUDRATE_DEFAULT;
    
    m_memset(&(registers->server_coils), 0 ,sizeof(registers->server_coils));
    m_memset(&(registers->server_input_register), 0 ,sizeof(registers->server_input_register));
    registers->server_input_register.serial_number      =   RTU_SERIAL_NUMBER_DEFAULT;
    registers->server_input_register.sensor_type        =   RTU_SENSOR_TYPE_DEFAULT;
    registers->server_holding_register.periode          =   TMR0_PeriodGet();
    registers->server_holding_register.curr_tail        =   CURR_TAIL;          // Corriente de corte (250mA * 10 = 2500)
    registers->server_holding_register.voltage_chrg_on  =   VOLTAGE_CHRG_ON;    // Voltage de reestablecimiento (12.7 V * 250 = 3175)
    registers->server_holding_register.beacon           =   0;                  // TRUE beacon on, FALSE beacon off
    registers->server_input_register.chrg               =   1;  
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

