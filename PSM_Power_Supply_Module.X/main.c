#include "mcc_generated_files/system/system.h"
#include <xc.h>
#include <stdio.h>  // Include the standard I/O library
#include <string.h> // Include the string library
#include "nanomodbus.h" 
#include "modbus_imp.h" 
#include "nvm_config.h" 
#include "measure.h"

// Modbus Variables
mod_bus_registers modbus_data;      // Coils, Holding Registers, Input Registers
holding_register prev_holding_regs; // Store the Holdding Registers´ previous values upon modbus commands
nmbs_t nmbs;                        // Main Server Structure
nmbs_platform_conf platform_conf;   // Platform Specific Config
nmbs_callbacks callbacks;           // Structure containing callback functions to be executed upon Modbus commands
adc_result_t vpanel = 0;

int main(void)
{
    SYSTEM_Initialize();

    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts 
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global and Peripheral Interrupts 
    // Use the following macros to: 

    // Enable the Global Interrupts 
    INTERRUPT_GlobalInterruptEnable(); 

    // Disable the Global Interrupts 
    //INTERRUPT_GlobalInterruptDisable(); 

    // Enable the Peripheral Interrupts 
    INTERRUPT_PeripheralInterruptEnable(); 

    // Disable the Peripheral Interrupts 
    //INTERRUPT_PeripheralInterruptDisable(); 
    
    sensor_data_t   panel_data;
    sensor_data_t   battery_data;
    sensor_data_t   cons_data;
    
    m_memset(&panel_data, 0, sizeof(sensor_data_t));
    m_memset(&battery_data, 0, sizeof(sensor_data_t));
    m_memset(&cons_data, 0, sizeof(sensor_data_t));
    
    __delay_ms(15);
    
    // Load default values into each modbus register
    default_values_register(&modbus_data);
    
    modbus_data.server_input_register.panel_data    = &panel_data;
    modbus_data.server_input_register.battery_data  = &battery_data;
    modbus_data.server_input_register.cons_data     = &cons_data;
    modbus_data.server_input_register.chrg          = 1;
    
    // Load default values into each modbus holding register (shadow copy)
    set_holding_regs_to_default(&prev_holding_regs);
    
    // Link Callback functions for modbus commands
    callbacks.read_holding_registers    = handler_read_holding_registers;
    callbacks.read_input_registers      = handler_read_input_registers; 
    callbacks.write_single_coil         = handle_write_single_coil;
    callbacks.write_single_register     = handle_write_single_register;
    
    // Platform config for Modbus RTU
    platform_conf.transport = NMBS_TRANSPORT_RTU;  // Modbus on RTU protocol
    platform_conf.read = read_serial;              // Link our modbus_imp read_serial function to the read from UART operation
    platform_conf.write = write_serial;            // Link our modbus_imp write_serial function to the write to UART operation
    platform_conf.arg = &(modbus_data);            // Pass out modbus registers instance as a parameter
    
    // Create Modbus Server Instance. Initialize nanomodbus stack as an RTU slave with the defined address in modbus_Data
    nmbs_error err = nmbs_server_create(
        &nmbs,                                              // Main Server Structure
        modbus_data.server_holding_register.addr_slave,     // Slave Address
        &platform_conf,                                     // Platform specific config
        &callbacks                                          // Callback functions to be executed upon modbus commands
    );
    
    // Check for critical errors
    if (err != NMBS_ERROR_NONE) 
    {
        // check_error_modbus(err)  // Handle error
        //while(1){}                  // Halt if unable to create modbus server 
    }
      
    //modbus_data.server_input_register.panel_data->voltage.hist[0] = 999;
    modbus_data.server_input_register.panel_data->voltage.hist[0] = 999;
    panel_data.voltage.hist[1] = 888;

    
    while(1)
    {
        err = nmbs_server_poll(&nmbs);
        if (err != NMBS_ERROR_NONE)  // Basic runtime error handling
        {
            // check_error_modbus(err)  // Handle error
        }
        else
        {
            // Handle changes in Holding Registers
            holding_register_change_handler(&modbus_data, &prev_holding_regs, &nmbs); 
            
            // Handle changes in coil registers (perform measurements according to which coil was written))
            if(nmbs_bitfield_read(modbus_data.server_coils.coils, 0) || nmbs_bitfield_read(modbus_data.server_coils.coils, 1))
            {
                // Meassure panel Test
                modbus_data.server_input_register.panel_data->voltage.hist[0] = get_voltage_measurement(VPMON,modbus_data.server_holding_register.panel_volt_calib_factor);
            }
            if(nmbs_bitfield_read(modbus_data.server_coils.coils, 0) || nmbs_bitfield_read(modbus_data.server_coils.coils, 2))
            {
               // Measure battery
            }
            if(nmbs_bitfield_read(modbus_data.server_coils.coils, 0) || nmbs_bitfield_read(modbus_data.server_coils.coils, 3))
            {
                // Measure consumption
            }
            m_memset(modbus_data.server_coils.coils, 0, sizeof(modbus_data.server_coils.coils));    // Reset all coils to 0
            } 
    }    
}
