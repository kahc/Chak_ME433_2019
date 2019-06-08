#include "i2c_accelerometer.h"
#include "i2c_master_noint.h"

// define addresses
#define accelerometer_address 0b1101010      // address, pull SDO low
#define reg_WHO_AM_I 0x0F       // equal to 0b01101001
#define reg_CTRL1_XL 0x10
#define reg_CTRL2_G 0x11
#define reg_CTRL3_C 0x12
#define reg_OUT_TEMP_L 0x20     // beginning of all 14 datapoints
#define reg_OUTX_L_G 0x22       // beginning of gyroscope
#define reg_OUTX_L_XL 0x28      // beginning of accelerometer

void init_accelerometer(){
    i2c_master_setup();
}

void write_accelerometer(unsigned char reg, unsigned char value){
    i2c_master_start();
    i2c_master_send(accelerometer_address << 1 | 0);
    i2c_master_send(reg);
    i2c_master_send(value);
    i2c_master_stop();
}
unsigned char read_accelerometer(unsigned char reg){
    // move to correct register
    i2c_master_start();
    i2c_master_send(accelerometer_address << 1 | 0);
    i2c_master_send(reg);
    i2c_master_restart();
    
    // read into local copy
    i2c_master_send(accelerometer_address << 1 | 1);
    char temp = i2c_master_recv();
    i2c_master_ack(1);
    i2c_master_stop();
    return temp;
}
void I2C_read_multiple(unsigned char reg, unsigned char * data, int length){
    // move to correct register
    i2c_master_start();
    i2c_master_send(accelerometer_address << 1 | 0);
    i2c_master_send(reg);
    i2c_master_restart();
    
    i2c_master_send(accelerometer_address << 1 | 1);
    int i;
    for(i=0; i<length-1; i++){
        data[i] = i2c_master_recv();
        i2c_master_ack(0);
    }
    data[i] = i2c_master_recv();
    i2c_master_ack(1);
    i2c_master_stop();
    
}