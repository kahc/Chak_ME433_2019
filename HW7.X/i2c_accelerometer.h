// using the MCP 23008 with the PIC32

#ifndef I2C_PIN_ACCEL_H    /* Guard against multiple inclusion */
#define I2C_PIN_ACCEL_H

// function declarations
void init_accelerometer();
void write_accelerometer(unsigned char reg, unsigned char level);
unsigned char read_accelerometer(unsigned char reg);
void I2C_read_multiple(unsigned char reg, unsigned char * data, int length);

#endif