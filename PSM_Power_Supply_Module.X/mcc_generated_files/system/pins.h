/**
 * Generated Pins header File
 * 
 * @file pins.h
 * 
 * @defgroup  pinsdriver Pins Driver
 * 
 * @brief This is generated driver header for pins. 
 *        This header file provides APIs for all pins selected in the GUI.
 *
 * @version Driver Version  3.0.0
*/

/*
© [2025] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

#ifndef PINS_H
#define PINS_H

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set IO_RC0 aliases
#define PWR_LED_TRIS                 TRISCbits.TRISC0
#define PWR_LED_LAT                  LATCbits.LATC0
#define PWR_LED_PORT                 PORTCbits.RC0
#define PWR_LED_WPU                  WPUCbits.WPUC0
#define PWR_LED_OD                   ODCONCbits.ODCC0
#define PWR_LED_ANS                  ANSELCbits.ANSC0
#define PWR_LED_SetHigh()            do { LATCbits.LATC0 = 1; } while(0)
#define PWR_LED_SetLow()             do { LATCbits.LATC0 = 0; } while(0)
#define PWR_LED_Toggle()             do { LATCbits.LATC0 = ~LATCbits.LATC0; } while(0)
#define PWR_LED_GetValue()           PORTCbits.RC0
#define PWR_LED_SetDigitalInput()    do { TRISCbits.TRISC0 = 1; } while(0)
#define PWR_LED_SetDigitalOutput()   do { TRISCbits.TRISC0 = 0; } while(0)
#define PWR_LED_SetPullup()          do { WPUCbits.WPUC0 = 1; } while(0)
#define PWR_LED_ResetPullup()        do { WPUCbits.WPUC0 = 0; } while(0)
#define PWR_LED_SetPushPull()        do { ODCONCbits.ODCC0 = 0; } while(0)
#define PWR_LED_SetOpenDrain()       do { ODCONCbits.ODCC0 = 1; } while(0)
#define PWR_LED_SetAnalogMode()      do { ANSELCbits.ANSC0 = 1; } while(0)
#define PWR_LED_SetDigitalMode()     do { ANSELCbits.ANSC0 = 0; } while(0)
// get/set IO_RC1 aliases
#define PWR_CHRG_TRIS                 TRISCbits.TRISC1
#define PWR_CHRG_LAT                  LATCbits.LATC1
#define PWR_CHRG_PORT                 PORTCbits.RC1
#define PWR_CHRG_WPU                  WPUCbits.WPUC1
#define PWR_CHRG_OD                   ODCONCbits.ODCC1
#define PWR_CHRG_ANS                  ANSELCbits.ANSC1
#define PWR_CHRG_SetHigh()            do { LATCbits.LATC1 = 1; } while(0)
#define PWR_CHRG_SetLow()             do { LATCbits.LATC1 = 0; } while(0)
#define PWR_CHRG_Toggle()             do { LATCbits.LATC1 = ~LATCbits.LATC1; } while(0)
#define PWR_CHRG_GetValue()           PORTCbits.RC1
#define PWR_CHRG_SetDigitalInput()    do { TRISCbits.TRISC1 = 1; } while(0)
#define PWR_CHRG_SetDigitalOutput()   do { TRISCbits.TRISC1 = 0; } while(0)
#define PWR_CHRG_SetPullup()          do { WPUCbits.WPUC1 = 1; } while(0)
#define PWR_CHRG_ResetPullup()        do { WPUCbits.WPUC1 = 0; } while(0)
#define PWR_CHRG_SetPushPull()        do { ODCONCbits.ODCC1 = 0; } while(0)
#define PWR_CHRG_SetOpenDrain()       do { ODCONCbits.ODCC1 = 1; } while(0)
#define PWR_CHRG_SetAnalogMode()      do { ANSELCbits.ANSC1 = 1; } while(0)
#define PWR_CHRG_SetDigitalMode()     do { ANSELCbits.ANSC1 = 0; } while(0)
// get/set IO_RC2 aliases
#define UVP_TRIS                 TRISCbits.TRISC2
#define UVP_LAT                  LATCbits.LATC2
#define UVP_PORT                 PORTCbits.RC2
#define UVP_WPU                  WPUCbits.WPUC2
#define UVP_OD                   ODCONCbits.ODCC2
#define UVP_ANS                  ANSELCbits.ANSC2
#define UVP_SetHigh()            do { LATCbits.LATC2 = 1; } while(0)
#define UVP_SetLow()             do { LATCbits.LATC2 = 0; } while(0)
#define UVP_Toggle()             do { LATCbits.LATC2 = ~LATCbits.LATC2; } while(0)
#define UVP_GetValue()           PORTCbits.RC2
#define UVP_SetDigitalInput()    do { TRISCbits.TRISC2 = 1; } while(0)
#define UVP_SetDigitalOutput()   do { TRISCbits.TRISC2 = 0; } while(0)
#define UVP_SetPullup()          do { WPUCbits.WPUC2 = 1; } while(0)
#define UVP_ResetPullup()        do { WPUCbits.WPUC2 = 0; } while(0)
#define UVP_SetPushPull()        do { ODCONCbits.ODCC2 = 0; } while(0)
#define UVP_SetOpenDrain()       do { ODCONCbits.ODCC2 = 1; } while(0)
#define UVP_SetAnalogMode()      do { ANSELCbits.ANSC2 = 1; } while(0)
#define UVP_SetDigitalMode()     do { ANSELCbits.ANSC2 = 0; } while(0)
// get/set IO_RC3 aliases
#define RW_485_TRIS                 TRISCbits.TRISC3
#define RW_485_LAT                  LATCbits.LATC3
#define RW_485_PORT                 PORTCbits.RC3
#define RW_485_WPU                  WPUCbits.WPUC3
#define RW_485_OD                   ODCONCbits.ODCC3
#define RW_485_ANS                  ANSELCbits.ANSC3
#define RW_485_SetHigh()            do { LATCbits.LATC3 = 1; } while(0)
#define RW_485_SetLow()             do { LATCbits.LATC3 = 0; } while(0)
#define RW_485_Toggle()             do { LATCbits.LATC3 = ~LATCbits.LATC3; } while(0)
#define RW_485_GetValue()           PORTCbits.RC3
#define RW_485_SetDigitalInput()    do { TRISCbits.TRISC3 = 1; } while(0)
#define RW_485_SetDigitalOutput()   do { TRISCbits.TRISC3 = 0; } while(0)
#define RW_485_SetPullup()          do { WPUCbits.WPUC3 = 1; } while(0)
#define RW_485_ResetPullup()        do { WPUCbits.WPUC3 = 0; } while(0)
#define RW_485_SetPushPull()        do { ODCONCbits.ODCC3 = 0; } while(0)
#define RW_485_SetOpenDrain()       do { ODCONCbits.ODCC3 = 1; } while(0)
#define RW_485_SetAnalogMode()      do { ANSELCbits.ANSC3 = 1; } while(0)
#define RW_485_SetDigitalMode()     do { ANSELCbits.ANSC3 = 0; } while(0)
// get/set IO_RC4 aliases
#define ALS_TRIS                 TRISCbits.TRISC4
#define ALS_LAT                  LATCbits.LATC4
#define ALS_PORT                 PORTCbits.RC4
#define ALS_WPU                  WPUCbits.WPUC4
#define ALS_OD                   ODCONCbits.ODCC4
#define ALS_ANS                  ANSELCbits.ANSC4
#define ALS_SetHigh()            do { LATCbits.LATC4 = 1; } while(0)
#define ALS_SetLow()             do { LATCbits.LATC4 = 0; } while(0)
#define ALS_Toggle()             do { LATCbits.LATC4 = ~LATCbits.LATC4; } while(0)
#define ALS_GetValue()           PORTCbits.RC4
#define ALS_SetDigitalInput()    do { TRISCbits.TRISC4 = 1; } while(0)
#define ALS_SetDigitalOutput()   do { TRISCbits.TRISC4 = 0; } while(0)
#define ALS_SetPullup()          do { WPUCbits.WPUC4 = 1; } while(0)
#define ALS_ResetPullup()        do { WPUCbits.WPUC4 = 0; } while(0)
#define ALS_SetPushPull()        do { ODCONCbits.ODCC4 = 0; } while(0)
#define ALS_SetOpenDrain()       do { ODCONCbits.ODCC4 = 1; } while(0)
#define ALS_SetAnalogMode()      do { ANSELCbits.ANSC4 = 1; } while(0)
#define ALS_SetDigitalMode()     do { ANSELCbits.ANSC4 = 0; } while(0)
// get/set IO_RC6 aliases
#define IO_RC6_TRIS                 TRISCbits.TRISC6
#define IO_RC6_LAT                  LATCbits.LATC6
#define IO_RC6_PORT                 PORTCbits.RC6
#define IO_RC6_WPU                  WPUCbits.WPUC6
#define IO_RC6_OD                   ODCONCbits.ODCC6
#define IO_RC6_ANS                  ANSELCbits.ANSC6
#define IO_RC6_SetHigh()            do { LATCbits.LATC6 = 1; } while(0)
#define IO_RC6_SetLow()             do { LATCbits.LATC6 = 0; } while(0)
#define IO_RC6_Toggle()             do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define IO_RC6_GetValue()           PORTCbits.RC6
#define IO_RC6_SetDigitalInput()    do { TRISCbits.TRISC6 = 1; } while(0)
#define IO_RC6_SetDigitalOutput()   do { TRISCbits.TRISC6 = 0; } while(0)
#define IO_RC6_SetPullup()          do { WPUCbits.WPUC6 = 1; } while(0)
#define IO_RC6_ResetPullup()        do { WPUCbits.WPUC6 = 0; } while(0)
#define IO_RC6_SetPushPull()        do { ODCONCbits.ODCC6 = 0; } while(0)
#define IO_RC6_SetOpenDrain()       do { ODCONCbits.ODCC6 = 1; } while(0)
#define IO_RC6_SetAnalogMode()      do { ANSELCbits.ANSC6 = 1; } while(0)
#define IO_RC6_SetDigitalMode()     do { ANSELCbits.ANSC6 = 0; } while(0)
// get/set IO_RC7 aliases
#define IO_RC7_TRIS                 TRISCbits.TRISC7
#define IO_RC7_LAT                  LATCbits.LATC7
#define IO_RC7_PORT                 PORTCbits.RC7
#define IO_RC7_WPU                  WPUCbits.WPUC7
#define IO_RC7_OD                   ODCONCbits.ODCC7
#define IO_RC7_ANS                  ANSELCbits.ANSC7
#define IO_RC7_SetHigh()            do { LATCbits.LATC7 = 1; } while(0)
#define IO_RC7_SetLow()             do { LATCbits.LATC7 = 0; } while(0)
#define IO_RC7_Toggle()             do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define IO_RC7_GetValue()           PORTCbits.RC7
#define IO_RC7_SetDigitalInput()    do { TRISCbits.TRISC7 = 1; } while(0)
#define IO_RC7_SetDigitalOutput()   do { TRISCbits.TRISC7 = 0; } while(0)
#define IO_RC7_SetPullup()          do { WPUCbits.WPUC7 = 1; } while(0)
#define IO_RC7_ResetPullup()        do { WPUCbits.WPUC7 = 0; } while(0)
#define IO_RC7_SetPushPull()        do { ODCONCbits.ODCC7 = 0; } while(0)
#define IO_RC7_SetOpenDrain()       do { ODCONCbits.ODCC7 = 1; } while(0)
#define IO_RC7_SetAnalogMode()      do { ANSELCbits.ANSC7 = 1; } while(0)
#define IO_RC7_SetDigitalMode()     do { ANSELCbits.ANSC7 = 0; } while(0)
/**
 * @ingroup  pinsdriver
 * @brief GPIO and peripheral I/O initialization
 * @param none
 * @return none
 */
void PIN_MANAGER_Initialize (void);

/**
 * @ingroup  pinsdriver
 * @brief Interrupt on Change Handling routine
 * @param none
 * @return none
 */
void PIN_MANAGER_IOC(void);


#endif // PINS_H
/**
 End of File
*/