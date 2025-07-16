/*
 * File:   parameters.h
 * Author: Lucas
 *
 * Created on July 7, 2025, 6:13 PM
 */

#ifndef PARAMETERS_H
#define PARAMETERS_H

#ifdef __cplusplus
extern "C"
{
#endif

#define TMR0_TICK_US 32 // Each tick = 32 microseconds

#define BEACONS_ON 1
#define BEACONS_OFF 0
#define BEACONS_MODE_MANUAL 1
#define BEACONS_MODE_AUTO 0    // Beacons will be controlled by the ALS signal (turn on beacons at night)
#define ALS_ADC_THRESHOLD 3071 // ADC value corresponding to 2.475V with Vref = 3.3V

    // ------------------------------------- Default Values for Modbus Holding Registers -------------------------------------

#define RTU_SERVER_ADDRESS_DEFAULT 4 // Our RTU address (Slave number 4) - Slaves can be 0 to 255
#define RTU_BAUDRATE_DEFAULT 9600    // Default Baud Rate for UART

// Ideal values for calibration factors
// V_Resolution = (VADC / 4095) * ((R_top + R_bottom) / R_bottom) | V = V_Resolution * VADC
#define DEFAULT_PANEL_VOLT_CALIB_FACTOR 564 // For 12 bit ADC - Resolution = 5,64 mV
#define DEFAULT_BATT_VOLT_CAL_FACTOR 379    // For 12 bit ADC - Resolution = 3,79 mV
#define DEFAULT_CONS_VOLT_CAL_FACTOR 379    // For 12 bit ADC - Resolution = 3,79 mV

// I_Resolution = VADC / (4095 * R_sense * Gain) |  I = VADC * I_Resolution
#define DEFAULT_PANEL_CURR_CALIB_FACTOR 322 // For 12 bit ADC - Resolution = 379 uA
#define DEFAULT_BATT_CURR_CALIB_FACTOR 732  // For 12 bit ADC - Resolution = 732 uA
#define DEFAULT_CONS_CURR_CALIB_FACTOR 732  // For 12 bit ADC - Resolution = 732 uA

// === THRESHOLD VALUES IN REAL UNITS ===
#define VOLTAGE_CHRG_OFF_mV 1350000 // upscaled 13.5 V
#define VOLTAGE_CHRG_ON_mV 1250000  // upscaled 12.5 V
#define CURR_TAIL_uA 100000         // upscaled 100 mA

    // #define VOLTAGE_CHRG_OFF 3375               // Cut-off voltage (13.5 V / 3.79 mV = 3563)
    // #define VOLTAGE_CHRG_ON 3298                // Re-enable voltage (12.5 V / 3.79 mV = 3298)
    // #define CURR_TAIL 137                       // Cut-off current (100 mA / 732 uA = 137)

#define VOLTAGE_CHRG_OFF ((uint16_t)(VOLTAGE_CHRG_OFF_mV / DEFAULT_BATT_VOLT_CAL_FACTOR)) // Cut-off voltage (13.5 V / 3.79 mV = 3563)
#define VOLTAGE_CHRG_ON ((uint16_t)(VOLTAGE_CHRG_ON_mV / DEFAULT_BATT_VOLT_CAL_FACTOR))   // Re-enable voltage (12.5 V / 3.79 mV = 3298)
#define CURR_TAIL ((uint16_t)(CURR_TAIL_uA / DEFAULT_BATT_CURR_CALIB_FACTOR))             // Cut-off current (100 mA / 732 uA = 137)

// -----------------------------------------------------------------------------------------------------------------------

// ------------------------------------- Default Values for Modbus Input Registers ---------------------------------------
#define RTU_SENSOR_TYPE_DEFAULT 700 // Sensor ID = 700, Power Supply Module
#define RTU_SERIAL_NUMBER_DEFAULT 1 // Apply proper serial number later
// -----------------------------------------------------------------------------------------------------------------------

// --------------------------------- Sizes to use in structs for Modbus Input Registers ----------------------------------
#define HIST_SIZE 8
#define MIN_SIZE 1
#define MAX_SIZE 1
#define MED_SIZE 1
// -----------------------------------------------------------------------------------------------------------------------

// ---------------------------------------- Modbus Addresses for Input Registers -----------------------------------------
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
    // -----------------------------------------------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif /* PARAMETERS_H */
