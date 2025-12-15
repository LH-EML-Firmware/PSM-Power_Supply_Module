/* 
 * File:   parameters.h
 * Author: Lucas
 *
 * Created on July 7, 2025, 6:13 PM
 */

#ifndef PARAMETERS_H
#define	PARAMETERS_H

#ifdef	__cplusplus
extern "C" {
#endif

#define TMR0_TICK_US 32  // Each tick = 32 microseconds. This is used to calculate the value to configure TMR0 period.
    
#define BEACONS_ON          1
#define BEACONS_OFF         0  
#define BEACONS_MODE_MANUAL 1    
#define BEACONS_MODE_AUTO   0     // Beacons will be controlled by the ALS signal (turn on beacons at night).
#define BEACONS_DUTY_BLINK  0     // In manual mode, when activated beacons will be turned on during 100ms every 1 second
#define BEACONS_DUTY_ON_OFF 1     // In manual mode, when beacons will be continuously ON or OFF according to the beacon holding register
#define UVP_MODE_MANUAL 1         // UVP is set / unset according to holding register 8.  
#define UVP_MODE_AUTO   0         // UVP is always on and the Under Voltage Protection is performed by hardware. 
#define CHRG_MODE_MANUAL 1        // CHRG is set / unset according to holding register 10.  
#define CHRG_MODE_AUTO   0        // CHRG is turned on/off according to the measured battery voltage and current
    
#define ALS_ADC_THRESHOLD   3071  // ADC value corresponding to 2.475V with Vref = 3.3V

// ------------------------------------- Default Values for Modbus Holding Registers -------------------------------------
   
#define RTU_SERVER_ADDRESS_DEFAULT  4       // Our RTU address (Slave number 4) - Slaves can be 0 to 255
#define RTU_BAUDRATE_DEFAULT        9600    // Default Baud Rate for UART
    
// ============================ CALIBRATION FACTORS ============================
// These define the ADC resolution for each measurement source.
// Units are expressed in:
//   - Voltage calibration factors: in microvolts per ADC count (µV/count) upscaled x100
//   - Current calibration factors: in microamps per ADC count (µA/count)
// These are used to convert raw ADC values into real-world units.
    
// V_Resolution = (VADC / 4095) * ((R_top + R_bottom) / R_bottom) | V[mV] = V_Resolution * ADC_Value
#define DEFAULT_PANEL_VOLT_CALIB_FACTOR     564         // For 12 bit ADC - V_Resolution = 5,64 mV (scaled ×100)
#define DEFAULT_BATT_VOLT_CAL_FACTOR        379         // For 12 bit ADC - V_Resolution = 3,79 mV (scaled ×100)
#define DEFAULT_CONS_VOLT_CAL_FACTOR        379         // For 12 bit ADC - V_Resolution = 3,79 mV (scaled ×100)
// V[Volts] =  ADC_Value * (V_Resolution/1000)
// V[Volts] =  ADC_Value * (VOLT_CALIB_FACTOR/100000)

// I_Resolution = VADC / (4095 * R_sense * Gain) |  I[uA] = I_Resolution * ADC_Value
#define DEFAULT_PANEL_CURR_CALIB_FACTOR     322           // For 12 bit ADC - I_Resolution = 322 uA
#define DEFAULT_BATT_CURR_CALIB_FACTOR      732           // For 12 bit ADC - I_Resolution = 732 uA
#define DEFAULT_CONS_CURR_CALIB_FACTOR      732           // For 12 bit ADC - I_Resolution = 732 uA
// I[mA] =  ADC_Value * (I_Resolution/1000)  
// I[mA] =  ADC_Value * (CURR_CALIB_FACTOR/1000)

// ==== REAL MAGNITUDE UNIT - UPSCALED - THRESHOLD VALUES ====
// You may set the threshold values here (upscale them as indicated). The ADC equivalents of these values will be calculated automatically
#define UPSCALED_VOLTAGE_CHRG_OFF           1350000        // 13.5 V upscaled (desired voltage[V] * 100000)
#define UPSCALED_VOLTAGE_CHRG_ON            1250000        // 12.5 V upscaled (desired voltage[V] * 100000)
#define UPSCALED_CURR_TAIL                  100000         // 100 mA upscaled (desired current[mA] * 1000)
    
// ==== Auto-calculated 12 bit ADC representations for the Threshold Values ====
// These are the ones used to compare with ADC measurements in order to take the corresponding action 

// * Instead of these:
//#define VOLTAGE_CHRG_OFF        ((uint16_t)(UPSCALED_VOLTAGE_CHRG_OFF / DEFAULT_BATT_VOLT_CAL_FACTOR))      // Cut-off voltage (13.5 V / 3.79 mV = 3562)
//#define VOLTAGE_CHRG_ON         ((uint16_t)(UPSCALED_VOLTAGE_CHRG_ON  / DEFAULT_BATT_VOLT_CAL_FACTOR))      // Re-enable voltage (12.5 V / 3.79 mV = 3298)
//#define CURR_TAIL               ((uint16_t)(UPSCALED_CURR_TAIL / DEFAULT_BATT_CURR_CALIB_FACTOR))  

// * we use these:
#define VOLTAGE_CHRG_OFF  ((uint16_t)((UPSCALED_VOLTAGE_CHRG_OFF + (DEFAULT_BATT_VOLT_CAL_FACTOR / 2)) / DEFAULT_BATT_VOLT_CAL_FACTOR))  // Cut-off voltage (13.5 V / 3.79 mV ? 3562)
#define VOLTAGE_CHRG_ON   ((uint16_t)((UPSCALED_VOLTAGE_CHRG_ON  + (DEFAULT_BATT_VOLT_CAL_FACTOR / 2)) / DEFAULT_BATT_VOLT_CAL_FACTOR))  // Re-enable voltage (12.5 V / 3.79 mV ? 3298)         
#define CURR_TAIL  ((uint16_t)((UPSCALED_CURR_TAIL + (DEFAULT_BATT_CURR_CALIB_FACTOR / 2)) / DEFAULT_BATT_CURR_CALIB_FACTOR)) // Cut-off current (100 mA / 732 uA ? 137)

// * so that the result is rounded up when the decimal part is 0.5 or higher => rounded_result = (numerator + denominator / 2) / denominator;
// =============================================================================
    
// Holding registers for serial number write operations     
#define SN_PASSWORD_CORRECT         8336    // This value must be written into holding register 19 in order to enable a serial number write.
#define SN_WRITE_TIMEOUT            15      // Duration (in seconds) for which the serial number write operation remains enabled after correct password entry
                                            // Keep in mind, this will control a counter inside the TMR0 interrupt so if the interrupt period changes, so will this duration
#define SNW_STATUS_IDLE             0 
#define SNW_STATUS_SUCCESS          1 
#define SNW_STATUS_WRONG_PASS       2 
#define SNW_STATUS_NOT_AUTHORIZED   3 
// -----------------------------------------------------------------------------------------------------------------------
    
// ------------------------------------- Default Values for Modbus Input Registers ---------------------------------------
#define RTU_SENSOR_TYPE_DEFAULT     101     // Sensor ID = 101, Power Supply Module
#define RTU_SERIAL_NUMBER_DEFAULT   1       // Apply proper serial number later 
#define RTU_CHRG_STATUS_DEFAULT     0       // Initially, charger is off.    
// -----------------------------------------------------------------------------------------------------------------------
    
// --------------------------------- Sizes to use in structs for Modbus Input Registers ----------------------------------
#define HIST_SIZE   8
#define MIN_SIZE    1
#define MAX_SIZE    1
#define MED_SIZE    1
// -----------------------------------------------------------------------------------------------------------------------
    
// ---------------------------------------- Modbus Addresses for Input Registers -----------------------------------------
// This modbus addresses are required for the handler_read_input_registers function
#define PANEL_HIST_VOLT_ADDR    3
#define PANEL_MAX_VOLT_ADDR     PANEL_HIST_VOLT_ADDR+HIST_SIZE 
#define PANEL_MIN_VOLT_ADDR     PANEL_MAX_VOLT_ADDR+MAX_SIZE
#define PANEL_MED_VOLT_ADDR     PANEL_MIN_VOLT_ADDR+MIN_SIZE

#define PANEL_HIST_CURR_ADDR    14   
#define PANEL_MAX_CURR_ADDR     PANEL_HIST_CURR_ADDR+HIST_SIZE
#define PANEL_MIN_CURR_ADDR     PANEL_MAX_CURR_ADDR+MAX_SIZE
#define PANEL_MED_CURR_ADDR     PANEL_MIN_CURR_ADDR+MIN_SIZE


#define BAT_HIST_VOLT_ADDR      25  
#define BAT_MAX_VOLT_ADDR       BAT_HIST_VOLT_ADDR+HIST_SIZE
#define BAT_MIN_VOLT_ADDR       BAT_MAX_VOLT_ADDR+MAX_SIZE
#define BAT_MED_VOLT_ADDR       BAT_MIN_VOLT_ADDR+MIN_SIZE

#define BAT_HIST_CURR_ADDR      36    
#define BAT_MAX_CURR_ADDR       BAT_HIST_CURR_ADDR+HIST_SIZE
#define BAT_MIN_CURR_ADDR       BAT_MAX_CURR_ADDR+MAX_SIZE
#define BAT_MED_CURR_ADDR       BAT_MIN_CURR_ADDR+MIN_SIZE


#define CONS_HIST_VOLT_ADDR      47   
#define CONS_MAX_VOLT_ADDR       CONS_HIST_VOLT_ADDR+HIST_SIZE
#define CONS_MIN_VOLT_ADDR       CONS_MAX_VOLT_ADDR+MAX_SIZE
#define CONS_MED_VOLT_ADDR       CONS_MIN_VOLT_ADDR+MIN_SIZE

#define CONS_HIST_CURR_ADDR      58 
#define CONS_MAX_CURR_ADDR       CONS_HIST_CURR_ADDR+HIST_SIZE
#define CONS_MIN_CURR_ADDR       CONS_MAX_CURR_ADDR+MAX_SIZE
#define CONS_MED_CURR_ADDR       CONS_MIN_CURR_ADDR+MIN_SIZE
// -----------------------------------------------------------------------------------------------------------------------

#ifdef	__cplusplus
}
#endif

#endif	/* PARAMETERS_H */

