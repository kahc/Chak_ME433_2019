// using the MCP4922 with the PIC32 through SPI

#ifndef SPI_DAC_H    /* Guard against multiple inclusion */
#define SPI_DAC_H

#include <xc.h>

// pin definitions
#define dac_cs LATBbits.LATB0

// function declarations
void init_SPI1();
char SPI1_IO(char write);
void setVoltage(char channel, int voltage);

#endif
