#include "i2c_accelerometer.h"
#include "i2c_master_noint.h"

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