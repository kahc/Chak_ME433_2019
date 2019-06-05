// contains pragma definitions for the PIC32
// also contains initializations for user button and LED

#ifndef PIC_CONFIG_H    /* Guard against multiple inclusion */
#define PIC_CONFIG_H

#include <xc.h>           // processor SFR definitions
#include <sys/attribs.h>  // __ISR macro

void board_setup();

// define some global constants for convenience
#define true 1
#define false 0

// name user pins
#define user_LED LATAbits.LATA0
#define user_button PORTAbits.RA1

#endif