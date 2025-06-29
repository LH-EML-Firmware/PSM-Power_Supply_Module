
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
    
// ------------------- Modbus Limits -------------------
#define COILS_ADDR_MAX          5
#define REGS_INPUT_ADDR_MAX     10
#define REGS_HOLDING_ADDR_MAX   7           // This must be the amount of holding registers + 1, otherwise modbus support fails
#define MAX_SLAVE_VALUE         255
#define MIN_SLAVE_VALUE         1

// ------------------------------------------- Coils -------------------------------------------
/* Coil Address | Function   (used to order history/max/min/mean voltage/current from panel/battery/consumption)
 * -------------|---------------------------------------------------------
 * 0            | Sense all measurements (panel, battery, consumption)
 * 1            | Sense only from the solar panel (voltage & current)
 * 2            | Sense only from the battery (voltage & current)
 * 3            | Sense only from the consumption/load (voltage & current)
 */
typedef struct  // A single nmbs_bitfield variable can keep 2000 coils
{
    nmbs_bitfield coils;
}coils;
// ---------------------------------------------------------------------------------------------

// ------------------------------------- Holding Registers -------------------------------------
// Default Values
#define RTU_SERVER_ADDRESS_DEFAULT  4       // Our RTU address (Slave number 4) - Slaves can be 0 to 255
#define RTU_BAUDRATE_DEFAULT        9600    // Default Baud Rate for UART
#define VOLTAGE_CHRG_OFF 3375               // Cut-off voltage (13.5 V * 250 = 3375)
#define VOLTAGE_CHRG_ON 3125                // Re-enable voltage (12.5 V * 250 = 3125)
#define CURR_TAIL 1000                      // Cut-off current (250 mA * 10 = 2500)

typedef struct
{
    uint16_t addr_slave;            // 40000 - Holding Register 0 - Slave Num
    uint16_t baudrate;              // 40001 - Holding Register 1 - COM Baudrate (9600 default))
    
    uint16_t periode;               // 40002 - Holding Register 2 - Measuring Periode
    uint16_t voltage_chrg_on;       // 40003 - Holding Register 3 - Re-enable voltage (12.5 V * 250 = 3125)
    uint16_t curr_tail;             // 40004 - Holding Register 4 - Cut-off current (250 mA * 10 = 2500)
    uint16_t beacon;                // 40005 - Holding Register 5 - Beacons ON/OFF
}holding_register;

// ---------------------------------------------------------------------------------------------

// -------------------------------------- Input Registers --------------------------------------
/* Input registers contain:
 * The sensor type (In this case defined as code 100 -> Energy Board)
 * The sensor�s/board�s serial number (in this case 1, later to be changed for a defined convention) */

// Default Values for Input Registers
#define RTU_SENSOR_TYPE_DEFAULT     700     // Sensor ID = 700, Power Supply Module
#define RTU_SERIAL_NUMBER_DEFAULT   1       // Apply proper serial number later

// Defines and structs for measurement data
#define HIST_SIZE   8
#define MIN_SIZE    1
#define MAX_SIZE    1
#define MED_SIZE    1

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

// Input registers allocation
typedef struct
{
    uint16_t sensor_type;       // 30000 - Input Register 0 - Code for phase/resonance/power sensor
    uint16_t serial_number;     // 30001 - Input Register 1 - Sensor�s serial number
       
    uint16_t chrg;              // 30002 - Input Register 2 - Charging Status 1 - CHARGING, 0 - NOT CHARGING))
       
    sensor_data_t *panel_data;   // Input Registers 3 to 24
    sensor_data_t *battery_data; // Input Registers 25 to 46
    sensor_data_t *cons_data;    // Input Registers 47 to 68
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

// Initializes Modbus holding register structures with default values.
void set_holding_regs_to_default(holding_register* regs);

// Initializes Modbus register structures with default values.
void default_values_register(mod_bus_registers* registers);

// Handles and updates the registers and the NVM when holding registers are written
void holding_register_change_handler(mod_bus_registers* registers,holding_register* prev_holding_regs, nmbs_t* nmbs); // nmbs_t* nmbs 

// Writes a single 16 bit value into the NVM without deleting the rest of the row
void single_16_bit_nvm_write(uint16_t value);

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