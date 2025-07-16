
#ifndef MODBUS_IMP_H
#define MODBUS_IMP_H

#ifdef __cplusplus
extern "C"
{
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
#define COILS_ADDR_MAX 5
#define REGS_INPUT_ADDR_MAX 69
// #define REGS_HOLDING_ADDR_MAX   14           // This must be the amount of holding registers + 1, otherwise modbus support fails
#define REGS_HOLDING_ADDR_MAX 16
#define MAX_SLAVE_VALUE 255
#define MIN_SLAVE_VALUE 1

    // ------------------------------------------- Coils -------------------------------------------
    /* Coil Address | Function   (used to order history/max/min/mean voltage/current from panel/battery/consumption)
     * -------------|---------------------------------------------------------
     * 0            | Sense all measurements (panel, battery, consumption)
     * 1            | Sense only from the solar panel (voltage & current)
     * 2            | Sense only from the battery (voltage & current)
     * 3            | Sense only from the consumption/load (voltage & current)
     */
    typedef struct // A single nmbs_bitfield variable can keep 2000 coils
    {
        nmbs_bitfield coils;
    } coils;
// ---------------------------------------------------------------------------------------------

// ------------------------------------- Holding Registers -------------------------------------
// Default Values
#define RTU_SERVER_ADDRESS_DEFAULT 4 // Our RTU address (Slave number 4) - Slaves can be 0 to 255
#define RTU_BAUDRATE_DEFAULT 9600    // Default Baud Rate for UART

#define VOLTAGE_CHRG_OFF 3375 // Cut-off voltage (13.5 V * 250 = 3375)
#define VOLTAGE_CHRG_ON 3125  // Re-enable voltage (12.5 V * 250 = 3125)
#define CURR_TAIL 1000        // Cut-off current (250 mA * 10 = 2500)

// Ideal values for calibration factors

// Voltage: V_adc = Vin * (R_bottom / (R_top + R_bottom)), so:
// Vin = (adc * Vref) / 4095 * ((R_top + R_bottom) / R_bottom) = (adc * voltage_calib_factor) / 4095
#define DEFAULT_PANEL_VOLT_CALIB_FACTOR 23.1f // Using R17 = 120k, R20 = 20k, voltage_calib_factor = 3.3 * (140k / 20k)
#define DEFAULT_BATT_VOLT_CAL_FACTOR 15.52f   // Using R24 = 100k, R28 = 27k, voltage_calib_factor = 3.3 * (127k / 27k)
#define DEFAULT_CONS_VOLT_CAL_FACTOR 15.52f   // Using R39 = 100k, R41 = 27k, voltage_calib_factor = 3.3 * (127k / 27k)

// Current: V_adc = I_load * R_sense * Gain, so:
// I = (adc * Vref) / (4095 * R_sense * Gain) = (adc * current_calib_factor) / 4095
#define DEFAULT_PANEL_CURR_CALIB_FACTOR 3.3f // Using R_sense = 0.1?, Gain = 10, current_calib_factor = 3.3 / (0.1 * 10)
#define DEFAULT_BATT_CURR_CALIB_FACTOR 1.5f  // Using R_sense = 0.22?, Gain = 10, current_calib_factor = 3.3 / (0.22 * 10)
#define DEFAULT_CONS_CURR_CALIB_FACTOR 3.3f  // Using R_sense = 0.1?, Gain = 10, current_calib_factor = 3.3 / (0.1 * 10)

    typedef struct
    {
        uint16_t addr_slave; // 40000 - Holding Register 0  - Slave Num
        uint16_t baudrate;   // 40001 - Holding Register 1  - COM Baudrate (9600 default))

        uint16_t periode;         // 40002 - Holding Register 2  - Measuring Periode
        uint16_t voltage_chrg_on; // 40003 - Holding Register 3  - Re-enable voltage (12.5 V * 250 = 3125)
        uint16_t curr_tail;       // 40004 - Holding Register 4  - Cut-off current (250 mA * 10 = 2500)
        uint16_t beacon;          // 40005 - Holding Register 5  - Beacons ON/OFF
        uint16_t beacon_mode;     // 40006 - Holding Register 6  - Beacon mode: Auto or manual

        uint16_t panel_volt_calib_factor; // 40007 - Holding Register 7  - Panel Voltage Calibration Factor
        uint16_t batt_volt_calib_factor;  // 40008 - Holding Register 8  - Battery Voltage Calibration Factor
        uint16_t cons_volt_calib_factor;  // 40009 - Holding Register 9  - Consumption Voltage Calibration Factor
        uint16_t panel_curr_calib_factor; // 40010 - Holding Register 10  - Panel Current Calibration Factor
        uint16_t batt_curr_calib_factor;  // 40011 - Holding Register 11 - Battery Current Calibration Factor
        uint16_t cons_curr_calib_factor;  // 40012 - Holding Register 12 - Consumption Current Calibration Factor

        uint16_t bat_curr_plus;  // 13
        uint16_t bat_curr_minus; // 14
    } holding_register;

// ---------------------------------------------------------------------------------------------

// -------------------------------------- Input Registers --------------------------------------
/* Input registers contain:
 * The sensor type (In this case defined as code 100 -> Energy Board)
 * The sensor�s/board�s serial number (in this case 1, later to be changed for a defined convention) */

// Default Values for Input Registers
#define RTU_SENSOR_TYPE_DEFAULT 700 // Sensor ID = 700, Power Supply Module
#define RTU_SERIAL_NUMBER_DEFAULT 1 // Apply proper serial number later

// Defines and structs for measurement data
#define HIST_SIZE 8
#define MIN_SIZE 1
#define MAX_SIZE 1
#define MED_SIZE 1

// This modbus addresses are required for the handler_read_input_registers function
#define PANEL_HIST_VOLT_ADDR 3
#define PANEL_MAX_VOLT_ADDR PANEL_HIST_VOLT_ADDR + HIST_SIZE
#define PANEL_MIN_VOLT_ADDR PANEL_MAX_VOLT_ADDR + MAX_SIZE
#define PANEL_MED_VOLT_ADDR PANEL_MIN_VOLT_ADDR + MIN_SIZE

#define PANEL_HIST_CURR_ADDR 14
#define PANEL_MAX_CURR_ADDR PANEL_HIST_CURR_ADDR + HIST_SIZE
#define PANEL_MIN_CURR_ADDR PANEL_MAX_CURR_ADDR + MAX_SIZE
#define PANEL_MED_CURR_ADDR PANEL_MIN_CURR_ADDR + MIN_SIZE

#define BAT_HIST_VOLT_ADDR 25
#define BAT_MAX_VOLT_ADDR BAT_HIST_VOLT_ADDR + HIST_SIZE
#define BAT_MIN_VOLT_ADDR BAT_MAX_VOLT_ADDR + MAX_SIZE
#define BAT_MED_VOLT_ADDR BAT_MIN_VOLT_ADDR + MIN_SIZE

#define BAT_HIST_CURR_ADDR 36
#define BAT_MAX_CURR_ADDR BAT_HIST_CURR_ADDR + HIST_SIZE
#define BAT_MIN_CURR_ADDR BAT_MAX_CURR_ADDR + MAX_SIZE
#define BAT_MED_CURR_ADDR BAT_MIN_CURR_ADDR + MIN_SIZE

#define CONS_HIST_VOLT_ADDR 47
#define CONS_MAX_VOLT_ADDR CONS_HIST_VOLT_ADDR + HIST_SIZE
#define CONS_MIN_VOLT_ADDR CONS_MAX_VOLT_ADDR + MAX_SIZE
#define CONS_MED_VOLT_ADDR CONS_MIN_VOLT_ADDR + MIN_SIZE

#define CONS_HIST_CURR_ADDR 58
#define CONS_MAX_CURR_ADDR CONS_HIST_CURR_ADDR + HIST_SIZE
#define CONS_MIN_CURR_ADDR CONS_MAX_CURR_ADDR + MAX_SIZE
#define CONS_MED_CURR_ADDR CONS_MIN_CURR_ADDR + MIN_SIZE

    typedef struct
    {
        int16_t hist[HIST_SIZE];
        int16_t max;
        int16_t min;
        int16_t med;
    } measure_data_t;

    typedef struct
    {
        measure_data_t voltage;
        measure_data_t current;
    } sensor_data_t;

    // Input registers allocation
    typedef struct
    {
        uint16_t sensor_type;   // 30000 - Input Register 0 - Code for phase/resonance/power sensor
        uint16_t serial_number; // 30001 - Input Register 1 - Sensor�s serial number

        uint16_t chrg; // 30002 - Input Register 2 - Charging Status 1 - CHARGING, 0 - NOT CHARGING))

        sensor_data_t *panel_data;   // Input Registers 3 to 24
        sensor_data_t *battery_data; // Input Registers 25 to 46
        sensor_data_t *cons_data;    // Input Registers 47 to 68

    } input_register;
    // ---------------------------------------------------------------------------------------------

    // With all of this data the modbus registers struct is built
    typedef struct
    {
        coils server_coils;
        input_register server_input_register;
        holding_register server_holding_register;
    } mod_bus_registers; // This is what will be created and later accessed when using the read/write coil/holding_registers/input_registers handlers

    // ----------------------------------- Function prototypes ---------------------------------------

    // Initializes Modbus holding register structures with default values.
    void set_holding_regs_to_default(holding_register *regs);

    // Initializes Modbus register structures with default values.
    void default_values_register(mod_bus_registers *registers);

    // Handles and updates the registers and the NVM when holding registers are written
    void holding_register_change_handler(mod_bus_registers *registers, holding_register *prev_holding_regs, nmbs_t *nmbs); // nmbs_t* nmbs

    // Manage beacons according to the beacons value contained in the "beacons" modbus holding register
    void manage_beacons(uint16_t beacons);

    // Writes a single 16 bit value into the NVM without deleting the rest of the row
    void single_16_bit_nvm_write(uint16_t value);

    // Handles and processes Modbus error codes (currently commented out).
    void check_error_modbus(nmbs_error err);

    int32_t read_serial(uint8_t *buf, uint16_t count, int32_t byte_timeout_ms, void *arg); // Reads 'count' bytes from the serial port into 'buf' with a timeout.

    int32_t write_serial(const uint8_t *buf, uint16_t count, int32_t byte_timeout_ms, void *arg); // Writes 'count' bytes from 'buf' to the serial port with a timeout.

    nmbs_error handle_read_coils(uint16_t address, uint16_t quantity, nmbs_bitfield coils_out, uint8_t unit_id, void *arg); // Handles Modbus "Read Coils" requests by copying coil states to output buffer.

    nmbs_error handle_write_single_coil(uint16_t address, bool coils, uint8_t unit_id, void *arg); // Handles Modbus "Write Single Coil" requests by updating a single coil state.

    nmbs_error handler_read_input_registers(uint16_t address, uint16_t quantity, uint16_t *registers_out, uint8_t unit_id, void *arg); // Handles Modbus "Read Input Registers" requests by copying input register values to output buffer.

    nmbs_error handler_read_holding_registers(uint16_t address, uint16_t quantity, uint16_t *registers_out, uint8_t unit_id, void *arg); // Handles Modbus "Read Holding Registers" requests by copying holding register values to output buffer.

    nmbs_error handle_write_single_register(uint16_t address, const uint16_t *registers, uint8_t unit_id, void *arg); // Handles Modbus "Write Single Register" requests by updating a single holding register value.

    void check_error_modbus(nmbs_error err); // Handles and processes Modbus error codes (currently commented out).

    uint16_t TMR0_MillisecondsToPeriod(uint16_t ms); // Converts milliseconds (obtain from the modbus register) to TMR0 period value in order to use such value to set the TMR0 period.

    uint16_t TMR0_PeriodToMilliseconds(uint16_t period); // Converts TMR0 period value to milliseconds in order to use such value to set the modbus register.

    // ---------------------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif /* MODBUS_IMP_H */