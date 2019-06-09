#include "i2c_IMU.h"
#include "i2c_master_noint.h"

void init_IMU(){
    i2c_master_setup();
    // setup acclerometer 1.66kHz sampling, with 2g sensitivity and 100Hz filter
    write_IMU(reg_CTRL1_XL, 0b10000010);
    // setup gyroscope 1.66kHz sampling with 1000dps sensitivity
    write_IMU(reg_CTRL2_G, 0b10001000);
    // flip IF_INC bit to turn on sequential reads (on by default, just in case)
    write_IMU(reg_CTRL3_C, 0b00000100);
}

void write_IMU(unsigned char reg, unsigned char value){
    i2c_master_start();
    i2c_master_send(IMU_address << 1 | 0);
    i2c_master_send(reg);
    i2c_master_send(value);
    i2c_master_stop();
}
unsigned char read_IMU(unsigned char reg){
    // move to correct register
    i2c_master_start();
    i2c_master_send(IMU_address << 1 | 0);
    i2c_master_send(reg);
    i2c_master_restart();
    
    // read into local copy
    i2c_master_send(IMU_address << 1 | 1);
    char temp = i2c_master_recv();
    i2c_master_ack(1);
    i2c_master_stop();
    return temp;
}
void I2C_read_multiple(unsigned char reg, unsigned char * data, int length){
    // move to correct register
    i2c_master_start();
    i2c_master_send(IMU_address << 1 | 0);
    i2c_master_send(reg);
    i2c_master_restart();
    
    i2c_master_send(IMU_address << 1 | 1);
    int i;
    for(i=0; i<length-1; i++){
        data[i] = i2c_master_recv();
        i2c_master_ack(0);
    }
    data[i] = i2c_master_recv();
    i2c_master_ack(1);
    i2c_master_stop();
    
}