// using the MCP 23008 with the PIC32

#ifndef I2C_PIN_EXPANDER_H    /* Guard against multiple inclusion */
#define I2C_PIN_EXPANDER_H

// define addresses
#define expander_gpio 0x09      // expander port/gpio register address
#define expander_iodir 0x00     // expander io direction register address
#define expander_address 0b0100000      // expander i2c address

// function declarations
void init_expander();
void set_io_direction_expander(unsigned char pin, unsigned char direction);
void set_expander(unsigned char pin, unsigned char level);
unsigned char get_expander();

#endif