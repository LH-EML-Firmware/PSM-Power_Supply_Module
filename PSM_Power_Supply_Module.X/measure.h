#ifndef MEASURE_H
#define MEASURE_H

#include "mcc_generated_files/system/system.h"
#include <stdint.h>

/**
 * @brief Performs an ADC conversion on the specified channel and returns the raw result.
 *
 * Selects the specified ADC channel, waits briefly to allow the signal to settle,
 * and returns the unscaled 12-bit ADC result (0?4095).
 *
 * @param channel ADC channel to read (e.g. AN2, AN3, etc.)
 * @return uint16_t Raw 12-bit ADC result
 */

uint16_t get_measurement(adc_channel_t channel);

/**
 * @brief Computes all statistical metrics (max, min, median) for the voltage and current of a device.
 *
 * Applies `get_max()`, `get_min()`, and `get_med()` to both voltage and current measurement data
 * within a `sensor_data_t` structure.
 *
 * @param device Pointer to the sensor_data_t structure holding voltage and current history.
 */

void get_metrics(sensor_data_t *device);

/**
 * @brief Calculates the maximum value found in the measurement history.
 *
 * Iterates over the `hist[]` array of the given `measure_data_t` and stores the highest value in the `max` field.
 *
 * @param data Pointer to the measurement data structure (e.g. current or voltage) to analyze.
 */

void get_max(measure_data_t *data);

/**
 * @brief Calculates the minimum value found in the measurement history.
 *
 * Iterates over the `hist[]` array of the given `measure_data_t` and stores the lowest value in the `min` field.
 *
 * @param data Pointer to the measurement data structure (e.g. current or voltage) to analyze.
 */

void get_min(measure_data_t *data);

/**
 * @brief Calculates the average (mean) of all values in the measurement history.
 *
 * Sums all entries in the `hist[]` array and stores the integer average in the `med` field.
 *
 * @param data Pointer to the measurement data structure (e.g. current or voltage) to analyze.
 */

void get_med(measure_data_t *data);

/**
 * @brief Inserts a new value at the beginning of a fixed-length array, shifting existing values to the right.
 *
 * The newest value is placed at index 0, and older values are shifted one position toward the end.
 * The last (oldest) value is discarded if the array is full. This function is useful for implementing
 * a history buffer (non-circular).
 *
 * @param array Pointer to the array to update (typically a history buffer).
 * @param value New value to insert at the beginning of the array.
 * @param size_array Number of elements in the array.
 */

void push_array(uint16_t array[], uint16_t value, uint8_t size_array);

/**
 * @brief Determines whether ambient light indicates a daylight condition based on ADC input from ALS.
 *
 * Reads the analog light level from the ALS (Ambient Light Sensor) input using the ADC, and compares
 * the result to a predefined threshold corresponding to 2.475?V. If the measured voltage is equal to or
 * above the threshold, the function returns true (daylight). Otherwise, it returns false (nighttime).
 *
 * This function replaces hardware comparator logic with a flexible ADC-based check for light conditions.
 *
 * @return true if light level indicates daylight, false if it indicates nighttime.
 */

bool is_daylight(void);

#endif // MEASURE_H
