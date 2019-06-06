#include "i2c_accelerometer.h"
#include "i2c_master_noint.h"

// define addresses
#define accelerometer_address 0b0100000      // expander i2c address

void init_accelerometer(){
    i2c_master_setup();
}

void write_accelerometer(unsigned char reg, char value){
    i2c_master_start();
    i2c_master_send(accelerometer_address << 1 |0);
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
    i2c_master_send(reg << 1 | 1);
    char temp = i2c_master_recv();
    i2c_master_ack(1);
    i2c_master_stop();
    return temp;
}
void I2C_read_multiple(unsigned char address, unsigned char reg, unsigned char * data, int length){
    
}