
#ifndef MODBUS_IMP_H
#define	MODBUS_IMP_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include "nanomodbus.h"  

/*
 * Modbus Addressing Overview
 *
 * Function Codes and Address Ranges:
 * ----------------------------------
 * Coils (0x01, 0x05):              Addresses 00001?09999
 *   - Read/Write single-bit values (booleans)
 *
 * Discrete Inputs (status) (0x02): Addresses 10001?19999
 *   - Read-only single-bit inputs
 *
 * Input Registers (0x04):          Addresses 30001?39999
 *   - Read-only 16-bit data
 *
 * Holding Registers (0x03, 0x06): Addresses 40001?49999
 *   - Read/Write 16-bit data
 *
 * Note:
 * These address ranges are logical/semantic representations.
 * In actual Modbus requests, addresses are zero-based offsets.
 * For example, register 40001 corresponds to address 0 in a Modbus frame.
 */
    
#define COILS_ADDR_MAX          5           // Up to 5 Coil Registers, // No Input Status
#define REGS_INPUT_ADDR_MAX     69          // Up to 69 Input Registers
#define REGS_HOLDING_ADDR_MAX   6           // Up to 4 Holding Registers

#define MAX_SLAVE_VALUE              255    
#define MIN_SLAVE_VALUE              1  

// ------------------------------------------- Coils -------------------------------------------
 /* Coil Address | Function   (used to order history/max/min/mean voltage/current from panel/battery/consumption)
 * -------------|---------------------------------------------------------
 * 0            | Sense all measurements (panel, battery, consumption)
 * 1            | Sense only from the solar panel (voltage & current)
 * 2            | Sense only from the battery (voltage & current)
 * 3            | Sense only from the consumption/load (voltage & current)
 * 4            | 
 * 5            | */
typedef struct  // A single nmbs_bitfield variable can keep 2000 coils
{
    nmbs_bitfield coils;
}coils;
// ---------------------------------------------------------------------------------------------

// ------------------------------------- Holding Registers -------------------------------------
#define RTU_SERVER_ADDRESS_DEFAULT  4       // Our RTU address (Slave number 4) // Slave can be 0 t0 255
#define RTU_BAUDRATE_DEFAULT        9600

// EEPROM Configuration Flags (determine whether to use default (RAM) values or previously saved (EEPROM) ones.)    
#define IS_IN_MEMORY_VALUE          0x00    // Use defaults (RAM)
#define IS_IN_MEMORY_EPP_ADDR       0x7001  // EEPROM address for the flag  
// In memory addresses for the content that goes into holding registers
#define SLAVE_EPP_ADDR              0x7002
#define BAUDRATE_EPP_ADDR           0x7004
#define PERIODE_EPP_ADDR            0x7006
#define CURR_TAIL_EPP_ADDR          0x7008
#define VOLTAGE_CHRG_ON_EPP_ADDR    0x700A
#define BEACON_EPP_ADDR             0x700C

// Default Values for Holding Registers
#define VOLTAGE_CHRG_OFF    3375   // Voltage de corte (13.5 V * 250 = 3375)
#define VOLTAGE_CHRG_ON     3125   // Voltage de reestablecimiento (12.5 V * 250 = 3125)
#define CURR_TAIL           1000  // Corriente de corte (250mA * 10 = 2500)

typedef struct
{
    uint16_t addr_slave;            // Holding Register 0
    uint16_t baudrate;              // Holding Register 1
    
    uint16_t periode;               // Holding Register 2
    uint16_t voltage_chrg_on;       // Holding Register 3
    uint16_t curr_tail;             // Holding Register 4
    uint16_t beacon;                // Holding Register 5
}holding_register;
// ---------------------------------------------------------------------------------------------

// -------------------------------------- Input Registers --------------------------------------
/* Input registers contain:
 * The sensor type (In this case defined as code 100 -> Energy Board)
 * The sensor´s/board´s serial number (in this case 1, later to be changed for a defined convention) */

#define SENSOR_TYPE_ADDR        0
#define SERIAL_NUMBER_ADDR      1       // Input Register 0 

#define RTU_SERIAL_NUMBER_DEFAULT   1
#define RTU_SENSOR_TYPE_DEFAULT     100

// The next register tells whether the battery is charging or not 
#define CHARGE_ADDR             2       // Input Register 1

/* Then,
Each of the three sources (panel, battery, and consumption) stores measured voltage and current data in this format:
 * Voltage:                                            Current:
    - 8 historical values (latest readings)             - 8 historical values (latest readings)
    - 1 maximum value                                   - 1 maximum value
    - 1 minimum value                                   - 1 minimum value
    - 1 mean (average) value                            - 1 mean (average) value

This structure is repeated for both voltage and current measurements. */
                          
// Summary per source:       | Quantity  | Voltage | Current |
                          // |-----------|---------|---------|
#define HIST_SIZE   8     // | History   |   8     |    8    |
#define MIN_SIZE    1     // | Max       |   1     |    1    |
#define MAX_SIZE    1     // | Min       |   1     |    1    |
#define MED_SIZE    1     // | Mean      |   1     |    1    |
                          /*  -------------------------------
 Total per source: 22 registers × 3 sources = 66 input registers */

#define PANEL_HIST_VOLT_ADDR    3                                   // Input Registers 3 to 10
#define PANEL_MAX_VOLT_ADDR     PANEL_HIST_VOLT_ADDR+HIST_SIZE      // Input Register 11
#define PANEL_MIN_VOLT_ADDR     PANEL_MAX_VOLT_ADDR+MAX_SIZE        // Input Register 12
#define PANEL_MED_VOLT_ADDR     PANEL_MIN_VOLT_ADDR+MIN_SIZE        // Input Register 13

#define PANEL_HIST_CURR_ADDR    14                                  // Input Register 14 to 21
#define PANEL_MAX_CURR_ADDR     PANEL_HIST_CURR_ADDR+HIST_SIZE      // Input Register 22
#define PANEL_MIN_CURR_ADDR     PANEL_MAX_CURR_ADDR+MAX_SIZE        // Input Register 23
#define PANEL_MED_CURR_ADDR     PANEL_MIN_CURR_ADDR+MIN_SIZE        // Input Register 24

#define BAT_HIST_VOLT_ADDR      25                                  // Input Registers 25 to 32
#define BAT_MAX_VOLT_ADDR       BAT_HIST_VOLT_ADDR+HIST_SIZE        // Input Register 33
#define BAT_MIN_VOLT_ADDR       BAT_MAX_VOLT_ADDR+MAX_SIZE          // Input Register 34
#define BAT_MED_VOLT_ADDR       BAT_MIN_VOLT_ADDR+MIN_SIZE          // Input Register 35

#define BAT_HIST_CURR_ADDR      36                                  // Input Registers 36 to 43
#define BAT_MAX_CURR_ADDR       BAT_HIST_CURR_ADDR+HIST_SIZE        // Input Register 44
#define BAT_MIN_CURR_ADDR       BAT_MAX_CURR_ADDR+MAX_SIZE          // Input Register 45
#define BAT_MED_CURR_ADDR       BAT_MIN_CURR_ADDR+MIN_SIZE          // Input Register 46


#define CONS_HIST_VOLT_ADDR      47                                 // Input Registers 47 to 54
#define CONS_MAX_VOLT_ADDR       CONS_HIST_VOLT_ADDR+HIST_SIZE      // Input Register 55
#define CONS_MIN_VOLT_ADDR       CONS_MAX_VOLT_ADDR+MAX_SIZE        // Input Register 56
#define CONS_MED_VOLT_ADDR       CONS_MIN_VOLT_ADDR+MIN_SIZE        // Input Register 57

#define CONS_HIST_CURR_ADDR      58                                 // Input Registers 58 to 65
#define CONS_MAX_CURR_ADDR       CONS_HIST_CURR_ADDR+HIST_SIZE      // Input Register 66
#define CONS_MIN_CURR_ADDR       CONS_MAX_CURR_ADDR+MAX_SIZE        // Input Register 67
#define CONS_MED_CURR_ADDR       CONS_MIN_CURR_ADDR+MIN_SIZE        // Input Register 68

typedef struct
{
    int16_t hist[HIST_SIZE];
	int16_t max;
    int16_t min;
    int16_t med;
}measure_data_t;

typedef struct
{
	measure_data_t voltage;
    measure_data_t current;
}sensor_data_t;

typedef struct
{
    uint16_t        sensor_type;        // Input Register 0
    uint16_t        serial_number;      // Input Register 1
    
    uint16_t        chrg;               // Input Register 2
       
    sensor_data_t   *panel_data;        // Input Registers 3 to 24
    sensor_data_t   *battery_data;      // Input Registers 25 to 46
    sensor_data_t   *cons_data;         // Input Registers 47 to 68
}input_register;
// ---------------------------------------------------------------------------------------------

// With all of this data the modbus registers struct is built
typedef struct
{
    coils server_coils;
    input_register server_input_register;
    holding_register server_holding_register;
}mod_bus_registers;  // This is what will be created and later accessed when using the read/write coil/holding_registers/input_registers handlers

// ----------------------------------- Function prototypes --------------------------------------- 

// Initializes Modbus register structures with default values.
void default_values_register(mod_bus_registers* registers);

// Handles and processes Modbus error codes (currently commented out).
void check_error_modbus(nmbs_error err); 


int32_t read_serial(uint8_t* buf, uint16_t count, int32_t byte_timeout_ms, void* arg);  // Reads 'count' bytes from the serial port into 'buf' with a timeout.

int32_t write_serial(const uint8_t* buf, uint16_t count, int32_t byte_timeout_ms, void* arg); // Writes 'count' bytes from 'buf' to the serial port with a timeout.

nmbs_error handle_read_coils(uint16_t address, uint16_t quantity, nmbs_bitfield coils_out, uint8_t unit_id, void *arg); // Handles Modbus "Read Coils" requests by copying coil states to output buffer.

nmbs_error handle_write_single_coil(uint16_t address, bool coils, uint8_t unit_id, void *arg); // Handles Modbus "Write Single Coil" requests by updating a single coil state.

nmbs_error handler_read_input_registers(uint16_t address, uint16_t quantity, uint16_t* registers_out, uint8_t unit_id, void *arg); // Handles Modbus "Read Input Registers" requests by copying input register values to output buffer.

nmbs_error handler_read_holding_registers(uint16_t address, uint16_t quantity, uint16_t* registers_out, uint8_t unit_id, void *arg); // Handles Modbus "Read Holding Registers" requests by copying holding register values to output buffer.

nmbs_error handle_write_single_register(uint16_t address, const uint16_t* registers, uint8_t unit_id, void *arg); // Handles Modbus "Write Single Register" requests by updating a single holding register value.

void check_error_modbus(nmbs_error err); // Handles and processes Modbus error codes (currently commented out).

// ---------------------------------------------------------------------------------------------

#ifdef	__cplusplus
}
#endif

#endif	/* MODBUS_IMP_H */