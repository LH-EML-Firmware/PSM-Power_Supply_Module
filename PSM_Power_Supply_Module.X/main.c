#include "mcc_generated_files/system/system.h"
#include <xc.h>
#include <stdio.h>  // Include the standard I/O library
#include <string.h> // Include the string library
#include <stdbool.h>
#include "parameters.h" 
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

// Store charging previous status
uint16_t prev_chgr = 0;

// Functions
void measurement_cycle();

// Interrupt routines
void tmr0_interrupt_handler();
void tmr2_interrupt_handler();
void tmr4_interrupt_handler();

// Action enablers
uint8_t periodic_measurement = 0;

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
    
    TMR0_OverflowCallbackRegister(&tmr0_interrupt_handler);
    TMR2_OverflowCallbackRegister(&tmr2_interrupt_handler);
    TMR4_OverflowCallbackRegister(&tmr4_interrupt_handler);
    
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
    modbus_data.server_input_register.chrg          = 0;
    
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
    
    // Initially beacons are off and charger is off.
    PWR_LED_SetLow();
    PWR_CHR_SetLow(); // modbus_data.server_input_register.chrg is initially set to 0;
    prev_chgr = modbus_data.server_input_register.chrg; // Load prev_chrg with initial charging status
    UVP_SetHigh();    // UVP must be in open drain and set to high. Under Voltage Protection is performed by hardware. 
    
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
                // Measure panel voltage and current
                push_array(panel_data.voltage.hist, get_measurement(VPMON) , HIST_SIZE);        // Voltage
                push_array(panel_data.current.hist,get_measurement(IPMON), HIST_SIZE);          // Current
            }
            if(nmbs_bitfield_read(modbus_data.server_coils.coils, 0) || nmbs_bitfield_read(modbus_data.server_coils.coils, 2))
            {
               // Measure battery voltage and current
               push_array(battery_data.voltage.hist, get_measurement(VBMON) , HIST_SIZE);      // Voltage
               push_array(battery_data.current.hist,get_measurement(IBMONp), HIST_SIZE);       // Current
            }
            if(nmbs_bitfield_read(modbus_data.server_coils.coils, 0) || nmbs_bitfield_read(modbus_data.server_coils.coils, 3))
            {
                // Measure consumption voltage and current
                push_array(cons_data.voltage.hist, get_measurement(VCMON) , HIST_SIZE);         // Voltage
                push_array(cons_data.current.hist,get_measurement(ICMON), HIST_SIZE);           // Current
            }
            m_memset(modbus_data.server_coils.coils, 0, sizeof(modbus_data.server_coils.coils));    // Reset all coils to 0
        }
        
        // -------------------------------- Measure Panel/Battery/Consumption Voltage/Current every --------------------------------
        if(periodic_measurement)
        {
            // ------------------------------------------------- Panel -------------------------------------------------
            push_array(panel_data.voltage.hist, get_measurement(VPMON) , HIST_SIZE);        // Voltage
            push_array(panel_data.current.hist,get_measurement(IPMON), HIST_SIZE);          // Current
            // ---------------------------------------------------------------------------------------------------------
            
            // ------------------------------------------------ Battery ------------------------------------------------
            push_array(battery_data.voltage.hist, get_measurement(VBMON) , HIST_SIZE);      // Voltage
            // The battery's current might be possitive (if the battery is powering the system - discharge) 
            // or negative if the battery is being charged. During charging, IBMON+ (IBMONp) is 0. However,
            // IBMON- (IBMONm) is not 0 during discharge. Hence, the registers are updated with the corresponding
            // value (IBMON+ or IBMON-) according to the charging status.
            if(modbus_data.server_input_register.chrg == 1)
            {
                push_array(battery_data.current.hist,-get_measurement(IBMONm), HIST_SIZE);       // Current
            }
            else
            {
                push_array(battery_data.current.hist,get_measurement(IBMONp), HIST_SIZE);       // Current
            }
            // ---------------------------------------------------------------------------------------------------------
            
            // --------------------------------------------- Consumption -----------------------------------------------
            push_array(cons_data.voltage.hist, get_measurement(VCMON) , HIST_SIZE);         // Voltage
            push_array(cons_data.current.hist,get_measurement(ICMON), HIST_SIZE);           // Current
            // ---------------------------------------------------------------------------------------------------------
            
            // ------------------------------------------------ Metrics ------------------------------------------------
            get_metrics(&panel_data);
            get_metrics(&battery_data);
            get_metrics(&cons_data);
            // ---------------------------------------------------------------------------------------------------------
            
            periodic_measurement = 0;
        }
        // -------------------------------------------------------------------------------------------------------------------------
        
        // ------------------------------------------------- Charger Control -------------------------------------------------------
        if(modbus_data.server_holding_register.chrg_mode == CHRG_MODE_MANUAL) // Manual Control
        {
            if (modbus_data.server_holding_register.chrg == 1){PWR_CHR_SetHigh();}
            else{PWR_CHR_SetLow();}
        // -------------------------------------------------------------------------------------------------------------------------
        }
        else if (modbus_data.server_holding_register.chrg_mode == CHRG_MODE_AUTO) // Control Charging Status According to Battery Voltage and Current 
            // Update charging status
            if (modbus_data.server_input_register.battery_data->voltage.med > VOLTAGE_CHRG_OFF) // Once the battery this voltage:
            {
                if(battery_data.current.med > CURR_TAIL)
                {
                    modbus_data.server_input_register.chrg = 0; // Turn off the charger if the mean battery current exceeds the tail current.
                }                                               // Tail Current : Lowest charging current measured when a battery is nearing full charge.
            }
            else if (modbus_data.server_input_register.battery_data->voltage.med < VOLTAGE_CHRG_ON)
            {
                modbus_data.server_input_register.chrg = 1; // If the battery has dropped under this VOLTAGE_CHRG_ON, turn on the charger.
            }
            // Actually turn the charger ON or OFF.
            if (modbus_data.server_input_register.chrg != prev_chgr) // If the charging status has changed, turn on or off the charger according to it.
            {
                if (modbus_data.server_input_register.chrg)
                {
                    PWR_CHR_SetHigh();
                }
                else
                {
                    PWR_CHR_SetLow();
                }
                prev_chgr = modbus_data.server_input_register.chrg; // Update previous charging status with the current one.
            }  
        // -------------------------------------------------------------------------------------------------------------------------  
    }        
}


void tmr0_interrupt_handler()
{
    periodic_measurement = 1;
    if(modbus_data.server_holding_register.beacon_mode == BEACONS_MODE_AUTO)
    {
        if(is_daylight())
        {
            manage_beacons(BEACONS_OFF);
        }
        else    // Night time -> Turn on beacons
        {
            manage_beacons(BEACONS_ON);
        }
    }
}

void tmr2_interrupt_handler()
{
    PWR_LED_SetLow();
}

void tmr4_interrupt_handler()
{
    PWR_LED_SetHigh();
}

void manage_beacons(uint16_t beacons)  // Declared and called in modbus_imp, when setting registers to defalut or managing changes in registers.
{
    if(beacons == 1)
    {
        TMR4_Start();
        TMR2_Start();
    }
    else
    {
        PWR_LED_SetLow();
        TMR4_Stop();
        TMR2_Stop();
    }
}
