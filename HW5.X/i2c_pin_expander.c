#include "i2c_pin_expander.h"
#include "i2c_master_noint.h"

// keep local copy of important registers to avoid overwriting
// initialize with default values
char local_GPIO;
char local_IODIR;

void init_expander(){
    i2c_master_setup();
    local_GPIO = 0x00;
    local_IODIR = 0xFF;
}

void set_io_direction_expander(char pin, char direction){
    // edit local copy
    if(direction == 1){
        local_IODIR = local_IODIR | (1 << pin);
    }
    else{
        local_IODIR = local_IODIR & !(1 << pin);
    }
    
    // send via i2c
    i2c_master_start();
    i2c_master_send(expander_address << 1 |0);
    i2c_master_send(expander_iodir);
    i2c_master_send(local_IODIR);
    i2c_master_stop();
}

void set_expander(char pin, char level){
    // edit local copy
    if(level == 1){
        local_GPIO = local_GPIO | (0x1 << pin);
    }
    else{
        local_GPIO = local_GPIO & !(0x1 << pin);
    }
    
    // send via i2c
    i2c_master_start();
    i2c_master_send(expander_address << 1 |0);
    i2c_master_send(expander_gpio);
    i2c_master_send(local_GPIO);
    i2c_master_stop();
}

char get_expander(){
    // move to correct register
    i2c_master_start();
    i2c_master_send(expander_address << 1 | 0);
    i2c_master_send(expander_gpio);
    i2c_master_restart();
    
    // read into local copy
    i2c_master_send(expander_address << 1 | 1);
    local_GPIO = i2c_master_recv();
    i2c_master_ack(1);
    i2c_master_stop();
    return local_GPIO;
}
