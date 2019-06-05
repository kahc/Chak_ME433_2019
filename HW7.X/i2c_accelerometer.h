// using the MCP 23008 with the PIC32

#ifndef I2C_PIN_ACCEL_H    /* Guard against multiple inclusion */
#define I2C_PIN_ACCEL_H

// define addresses
#define expander_gpio 0x09      // expander port/gpio register address
#define expander_iodir 0x00     // expander io direction register address
#define expander_address 0b0100000      // expander i2c address

// function declarations
void init_accelerometer();
void write_accelerometer(unsigned char reg, char level);
unsigned char read_accelerometer();
void I2C_read_multiple(unsigned char address, unsigned char reg, unsigned char * data, int length);

#endif