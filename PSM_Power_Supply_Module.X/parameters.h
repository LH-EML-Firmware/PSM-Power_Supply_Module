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

#define BEACONS_ON          1
#define BEACONS_OFF         0  
#define BEACONS_MODE_MANUAL 1    
#define BEACONS_MODE_AUTO   0     // Beacons will be controlled by the ALS signal (turn on beacons at night) 
#define ALS_ADC_THRESHOLD   3071  // ADC value corresponding to 2.475V with Vref = 3.3V


#ifdef	__cplusplus
}
#endif

#endif	/* PARAMETERS_H */

