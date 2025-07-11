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

// get/set IO_RA2 aliases
#define VPMON_TRIS                 TRISAbits.TRISA2
#define VPMON_LAT                  LATAbits.LATA2
#define VPMON_PORT                 PORTAbits.RA2
#define VPMON_WPU                  WPUAbits.WPUA2
#define VPMON_OD                   ODCONAbits.ODCA2
#define VPMON_ANS                  ANSELAbits.ANSA2
#define VPMON_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
#define VPMON_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
#define VPMON_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define VPMON_GetValue()           PORTAbits.RA2
#define VPMON_SetDigitalInput()    do { TRISAbits.TRISA2 = 1; } while(0)
#define VPMON_SetDigitalOutput()   do { TRISAbits.TRISA2 = 0; } while(0)
#define VPMON_SetPullup()          do { WPUAbits.WPUA2 = 1; } while(0)
#define VPMON_ResetPullup()        do { WPUAbits.WPUA2 = 0; } while(0)
#define VPMON_SetPushPull()        do { ODCONAbits.ODCA2 = 0; } while(0)
#define VPMON_SetOpenDrain()       do { ODCONAbits.ODCA2 = 1; } while(0)
#define VPMON_SetAnalogMode()      do { ANSELAbits.ANSA2 = 1; } while(0)
#define VPMON_SetDigitalMode()     do { ANSELAbits.ANSA2 = 0; } while(0)
// get/set IO_RA4 aliases
#define IBMONp_TRIS                 TRISAbits.TRISA4
#define IBMONp_LAT                  LATAbits.LATA4
#define IBMONp_PORT                 PORTAbits.RA4
#define IBMONp_WPU                  WPUAbits.WPUA4
#define IBMONp_OD                   ODCONAbits.ODCA4
#define IBMONp_ANS                  ANSELAbits.ANSA4
#define IBMONp_SetHigh()            do { LATAbits.LATA4 = 1; } while(0)
#define IBMONp_SetLow()             do { LATAbits.LATA4 = 0; } while(0)
#define IBMONp_Toggle()             do { LATAbits.LATA4 = ~LATAbits.LATA4; } while(0)
#define IBMONp_GetValue()           PORTAbits.RA4
#define IBMONp_SetDigitalInput()    do { TRISAbits.TRISA4 = 1; } while(0)
#define IBMONp_SetDigitalOutput()   do { TRISAbits.TRISA4 = 0; } while(0)
#define IBMONp_SetPullup()          do { WPUAbits.WPUA4 = 1; } while(0)
#define IBMONp_ResetPullup()        do { WPUAbits.WPUA4 = 0; } while(0)
#define IBMONp_SetPushPull()        do { ODCONAbits.ODCA4 = 0; } while(0)
#define IBMONp_SetOpenDrain()       do { ODCONAbits.ODCA4 = 1; } while(0)
#define IBMONp_SetAnalogMode()      do { ANSELAbits.ANSA4 = 1; } while(0)
#define IBMONp_SetDigitalMode()     do { ANSELAbits.ANSA4 = 0; } while(0)
// get/set IO_RA5 aliases
#define IBMONm_TRIS                 TRISAbits.TRISA5
#define IBMONm_LAT                  LATAbits.LATA5
#define IBMONm_PORT                 PORTAbits.RA5
#define IBMONm_WPU                  WPUAbits.WPUA5
#define IBMONm_OD                   ODCONAbits.ODCA5
#define IBMONm_ANS                  ANSELAbits.ANSA5
#define IBMONm_SetHigh()            do { LATAbits.LATA5 = 1; } while(0)
#define IBMONm_SetLow()             do { LATAbits.LATA5 = 0; } while(0)
#define IBMONm_Toggle()             do { LATAbits.LATA5 = ~LATAbits.LATA5; } while(0)
#define IBMONm_GetValue()           PORTAbits.RA5
#define IBMONm_SetDigitalInput()    do { TRISAbits.TRISA5 = 1; } while(0)
#define IBMONm_SetDigitalOutput()   do { TRISAbits.TRISA5 = 0; } while(0)
#define IBMONm_SetPullup()          do { WPUAbits.WPUA5 = 1; } while(0)
#define IBMONm_ResetPullup()        do { WPUAbits.WPUA5 = 0; } while(0)
#define IBMONm_SetPushPull()        do { ODCONAbits.ODCA5 = 0; } while(0)
#define IBMONm_SetOpenDrain()       do { ODCONAbits.ODCA5 = 1; } while(0)
#define IBMONm_SetAnalogMode()      do { ANSELAbits.ANSA5 = 1; } while(0)
#define IBMONm_SetDigitalMode()     do { ANSELAbits.ANSA5 = 0; } while(0)
// get/set IO_RB4 aliases
#define VBMON_TRIS                 TRISBbits.TRISB4
#define VBMON_LAT                  LATBbits.LATB4
#define VBMON_PORT                 PORTBbits.RB4
#define VBMON_WPU                  WPUBbits.WPUB4
#define VBMON_OD                   ODCONBbits.ODCB4
#define VBMON_ANS                  ANSELBbits.ANSB4
#define VBMON_SetHigh()            do { LATBbits.LATB4 = 1; } while(0)
#define VBMON_SetLow()             do { LATBbits.LATB4 = 0; } while(0)
#define VBMON_Toggle()             do { LATBbits.LATB4 = ~LATBbits.LATB4; } while(0)
#define VBMON_GetValue()           PORTBbits.RB4
#define VBMON_SetDigitalInput()    do { TRISBbits.TRISB4 = 1; } while(0)
#define VBMON_SetDigitalOutput()   do { TRISBbits.TRISB4 = 0; } while(0)
#define VBMON_SetPullup()          do { WPUBbits.WPUB4 = 1; } while(0)
#define VBMON_ResetPullup()        do { WPUBbits.WPUB4 = 0; } while(0)
#define VBMON_SetPushPull()        do { ODCONBbits.ODCB4 = 0; } while(0)
#define VBMON_SetOpenDrain()       do { ODCONBbits.ODCB4 = 1; } while(0)
#define VBMON_SetAnalogMode()      do { ANSELBbits.ANSB4 = 1; } while(0)
#define VBMON_SetDigitalMode()     do { ANSELBbits.ANSB4 = 0; } while(0)
// get/set IO_RB5 aliases
#define VCMON_TRIS                 TRISBbits.TRISB5
#define VCMON_LAT                  LATBbits.LATB5
#define VCMON_PORT                 PORTBbits.RB5
#define VCMON_WPU                  WPUBbits.WPUB5
#define VCMON_OD                   ODCONBbits.ODCB5
#define VCMON_ANS                  ANSELBbits.ANSB5
#define VCMON_SetHigh()            do { LATBbits.LATB5 = 1; } while(0)
#define VCMON_SetLow()             do { LATBbits.LATB5 = 0; } while(0)
#define VCMON_Toggle()             do { LATBbits.LATB5 = ~LATBbits.LATB5; } while(0)
#define VCMON_GetValue()           PORTBbits.RB5
#define VCMON_SetDigitalInput()    do { TRISBbits.TRISB5 = 1; } while(0)
#define VCMON_SetDigitalOutput()   do { TRISBbits.TRISB5 = 0; } while(0)
#define VCMON_SetPullup()          do { WPUBbits.WPUB5 = 1; } while(0)
#define VCMON_ResetPullup()        do { WPUBbits.WPUB5 = 0; } while(0)
#define VCMON_SetPushPull()        do { ODCONBbits.ODCB5 = 0; } while(0)
#define VCMON_SetOpenDrain()       do { ODCONBbits.ODCB5 = 1; } while(0)
#define VCMON_SetAnalogMode()      do { ANSELBbits.ANSB5 = 1; } while(0)
#define VCMON_SetDigitalMode()     do { ANSELBbits.ANSB5 = 0; } while(0)
// get/set IO_RB6 aliases
#define ICMON_TRIS                 TRISBbits.TRISB6
#define ICMON_LAT                  LATBbits.LATB6
#define ICMON_PORT                 PORTBbits.RB6
#define ICMON_WPU                  WPUBbits.WPUB6
#define ICMON_OD                   ODCONBbits.ODCB6
#define ICMON_ANS                  ANSELBbits.ANSB6
#define ICMON_SetHigh()            do { LATBbits.LATB6 = 1; } while(0)
#define ICMON_SetLow()             do { LATBbits.LATB6 = 0; } while(0)
#define ICMON_Toggle()             do { LATBbits.LATB6 = ~LATBbits.LATB6; } while(0)
#define ICMON_GetValue()           PORTBbits.RB6
#define ICMON_SetDigitalInput()    do { TRISBbits.TRISB6 = 1; } while(0)
#define ICMON_SetDigitalOutput()   do { TRISBbits.TRISB6 = 0; } while(0)
#define ICMON_SetPullup()          do { WPUBbits.WPUB6 = 1; } while(0)
#define ICMON_ResetPullup()        do { WPUBbits.WPUB6 = 0; } while(0)
#define ICMON_SetPushPull()        do { ODCONBbits.ODCB6 = 0; } while(0)
#define ICMON_SetOpenDrain()       do { ODCONBbits.ODCB6 = 1; } while(0)
#define ICMON_SetAnalogMode()      do { ANSELBbits.ANSB6 = 1; } while(0)
#define ICMON_SetDigitalMode()     do { ANSELBbits.ANSB6 = 0; } while(0)
// get/set IO_RB7 aliases
#define IPMON_TRIS                 TRISBbits.TRISB7
#define IPMON_LAT                  LATBbits.LATB7
#define IPMON_PORT                 PORTBbits.RB7
#define IPMON_WPU                  WPUBbits.WPUB7
#define IPMON_OD                   ODCONBbits.ODCB7
#define IPMON_ANS                  ANSELBbits.ANSB7
#define IPMON_SetHigh()            do { LATBbits.LATB7 = 1; } while(0)
#define IPMON_SetLow()             do { LATBbits.LATB7 = 0; } while(0)
#define IPMON_Toggle()             do { LATBbits.LATB7 = ~LATBbits.LATB7; } while(0)
#define IPMON_GetValue()           PORTBbits.RB7
#define IPMON_SetDigitalInput()    do { TRISBbits.TRISB7 = 1; } while(0)
#define IPMON_SetDigitalOutput()   do { TRISBbits.TRISB7 = 0; } while(0)
#define IPMON_SetPullup()          do { WPUBbits.WPUB7 = 1; } while(0)
#define IPMON_ResetPullup()        do { WPUBbits.WPUB7 = 0; } while(0)
#define IPMON_SetPushPull()        do { ODCONBbits.ODCB7 = 0; } while(0)
#define IPMON_SetOpenDrain()       do { ODCONBbits.ODCB7 = 1; } while(0)
#define IPMON_SetAnalogMode()      do { ANSELBbits.ANSB7 = 1; } while(0)
#define IPMON_SetDigitalMode()     do { ANSELBbits.ANSB7 = 0; } while(0)
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
#define PWR_CHR_TRIS                 TRISCbits.TRISC1
#define PWR_CHR_LAT                  LATCbits.LATC1
#define PWR_CHR_PORT                 PORTCbits.RC1
#define PWR_CHR_WPU                  WPUCbits.WPUC1
#define PWR_CHR_OD                   ODCONCbits.ODCC1
#define PWR_CHR_ANS                  ANSELCbits.ANSC1
#define PWR_CHR_SetHigh()            do { LATCbits.LATC1 = 1; } while(0)
#define PWR_CHR_SetLow()             do { LATCbits.LATC1 = 0; } while(0)
#define PWR_CHR_Toggle()             do { LATCbits.LATC1 = ~LATCbits.LATC1; } while(0)
#define PWR_CHR_GetValue()           PORTCbits.RC1
#define PWR_CHR_SetDigitalInput()    do { TRISCbits.TRISC1 = 1; } while(0)
#define PWR_CHR_SetDigitalOutput()   do { TRISCbits.TRISC1 = 0; } while(0)
#define PWR_CHR_SetPullup()          do { WPUCbits.WPUC1 = 1; } while(0)
#define PWR_CHR_ResetPullup()        do { WPUCbits.WPUC1 = 0; } while(0)
#define PWR_CHR_SetPushPull()        do { ODCONCbits.ODCC1 = 0; } while(0)
#define PWR_CHR_SetOpenDrain()       do { ODCONCbits.ODCC1 = 1; } while(0)
#define PWR_CHR_SetAnalogMode()      do { ANSELCbits.ANSC1 = 1; } while(0)
#define PWR_CHR_SetDigitalMode()     do { ANSELCbits.ANSC1 = 0; } while(0)
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
#define RW_TRIS                 TRISCbits.TRISC3
#define RW_LAT                  LATCbits.LATC3
#define RW_PORT                 PORTCbits.RC3
#define RW_WPU                  WPUCbits.WPUC3
#define RW_OD                   ODCONCbits.ODCC3
#define RW_ANS                  ANSELCbits.ANSC3
#define RW_SetHigh()            do { LATCbits.LATC3 = 1; } while(0)
#define RW_SetLow()             do { LATCbits.LATC3 = 0; } while(0)
#define RW_Toggle()             do { LATCbits.LATC3 = ~LATCbits.LATC3; } while(0)
#define RW_GetValue()           PORTCbits.RC3
#define RW_SetDigitalInput()    do { TRISCbits.TRISC3 = 1; } while(0)
#define RW_SetDigitalOutput()   do { TRISCbits.TRISC3 = 0; } while(0)
#define RW_SetPullup()          do { WPUCbits.WPUC3 = 1; } while(0)
#define RW_ResetPullup()        do { WPUCbits.WPUC3 = 0; } while(0)
#define RW_SetPushPull()        do { ODCONCbits.ODCC3 = 0; } while(0)
#define RW_SetOpenDrain()       do { ODCONCbits.ODCC3 = 1; } while(0)
#define RW_SetAnalogMode()      do { ANSELCbits.ANSC3 = 1; } while(0)
#define RW_SetDigitalMode()     do { ANSELCbits.ANSC3 = 0; } while(0)
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