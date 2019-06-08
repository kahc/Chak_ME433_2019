// using the MCP 23008 with the PIC32

#ifndef I2C_PIN_ACCEL_H    /* Guard against multiple inclusion */
#define I2C_PIN_ACCEL_H

// define addresses
#define IMU_address 0b1101010      // address, pull SDO low
#define reg_WHO_AM_I 0x0F       // equal to 0b01101001
#define reg_CTRL1_XL 0x10
#define reg_CTRL2_G 0x11
#define reg_CTRL3_C 0x12
#define reg_OUT_TEMP_L 0x20     // beginning of all 14 datapoints
#define reg_OUTX_L_G 0x22       // beginning of gyroscope
#define reg_OUTX_L_XL 0x28      // beginning of accelerometer

// function declarations
void init_IMU();
void write_IMU(unsigned char reg, unsigned char level);
unsigned char read_IMU(unsigned char reg);
void I2C_read_multiple(unsigned char reg, unsigned char * data, int length);

#endif