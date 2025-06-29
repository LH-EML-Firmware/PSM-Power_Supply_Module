#ifndef MEASURE_H
#define MEASURE_H

#include "mcc_generated_files/system/system.h"
#include <stdint.h>

/**
 * @brief Reads voltage from the specified ADC channel and converts it using a calibration factor.
 *
 * @param channel ADC channel to read (e.g. AN2, AN3, etc.)
 * @param voltage_calib_factor Calibration factor accounting for voltage divider and VREF
 * @return float Calculated voltage in volts
 */
float get_voltage_measurement(adc_channel_t channel, float voltage_calib_factor);

/**
 * @brief Reads current from the specified ADC channel and converts it using a calibration factor.
 *
 * @param channel ADC channel to read
 * @param current_calib_factor Calibration factor derived from VREF / (R_SENSE * GAIN)
 * @return float Calculated current in amperes
 */
float get_current_measurement(adc_channel_t channel, float current_calib_factor);

#endif // MEASURE_H
