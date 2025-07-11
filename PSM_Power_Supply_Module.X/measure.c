#include "mcc_generated_files/system/system.h"
#include <xc.h>
#include <stdbool.h>
#include "parameters.h"
#include "modbus_imp.h"

uint16_t get_measurement(adc_channel_t channel)
{
    adc_result_t adc_value = 0;
    //float voltage_value = 0.0f;

    // Select channel and allow input to settle
    ADC_ChannelSelect(channel);
    __delay_us(5); // Give time for input to stabilize

    // Start and wait for ADC conversion
    ADC_ConversionStart();
    while (!ADC_IsConversionDone());

    // Read ADC result and convert to voltage
    adc_value = ADC_ConversionResultGet();
    //voltage_value = ((float)adc_value * voltage_calib_factor) / 4095.0f;
    
    return adc_value;
}

void get_max(measure_data_t *data)
{
    int16_t aux = INT16_MIN;          // INT16_MIN = -32768, standard macro defined in limits.h
    for(uint8_t i=0; i<HIST_SIZE; i++)        
        if(aux < data->hist[i])
            aux = data->hist[i];
    
    data->max = aux;   
}

void get_min(measure_data_t *data)
{    
    int16_t aux = INT16_MAX; 
    for(uint8_t i=0; i<HIST_SIZE; i++)        
        if(aux > data->hist[i])
            aux = data->hist[i];
    
    data->min = aux;
}

void get_med(measure_data_t *data)
{
    int32_t buff = 0;

    for(uint8_t i=0; i<HIST_SIZE; i++)
        buff += data->hist[i];
    
    data->med = buff/HIST_SIZE;  
}

void get_metrics(sensor_data_t *device)
{
    get_max(&device->current);    
    get_min(&device->current);    
    get_med(&device->current);
    get_max(&device->voltage);    
    get_min(&device->voltage);    
    get_med(&device->voltage);
}

void push_array(uint16_t array[], uint16_t value, uint8_t size_array)
{
  for(uint8_t i = size_array - 1; i > 0; i--)
    array[i] = array[i - 1];

  array[0] = value;
}

bool is_daylight(void)
{
    uint16_t adc_val = get_measurement(ALS);  // Use your existing function
    return adc_val >= ALS_ADC_THRESHOLD;
}
