#include "mcc_generated_files/system/system.h"
#include <xc.h>

float get_voltage_measurement(adc_channel_t channel, float voltage_calib_factor)
{
    adc_result_t adc_value = 0;
    float voltage_value = 0.0f;

    // Select channel and allow input to settle
    ADC_ChannelSelect(channel);
    __delay_us(5); // Give time for input to stabilize

    // Start and wait for ADC conversion
    ADC_ConversionStart();
    while (!ADC_IsConversionDone())
        ;

    // Read ADC result and convert to voltage
    adc_value = ADC_ConversionResultGet();
    voltage_value = ((float)adc_value * voltage_calib_factor) / 4095.0f;

    return voltage_value;
}

float get_current_measurement(adc_channel_t channel, float current_calib_factor)
{
    adc_result_t adc_value = 0;
    float current_value = 0.0f;

    // Select ADC channel and settle
    ADC_ChannelSelect(channel);
    __delay_us(5);

    // Start conversion
    ADC_ConversionStart();
    while (!ADC_IsConversionDone())
        ;

    // Read ADC value and calculate current
    adc_value = ADC_ConversionResultGet();
    current_value = ((float)adc_value * current_calib_factor) / 4095.0f;

    return current_value;
}
