#include "config_bits.h"
#include "PIC_config.h"
#include "i2c_pin_expander.h"
#include "i2c_master_noint.h"

int main(){
    board_setup();
    init_expander();

    // expander is default input, set led to output
    // set_io_direction_expander(1, 0);
    //set_expander(1,1);
    
    set_io_direction_expander(0,0);
    set_expander(0,1);
    
    char expander_pin_7 = 0;
    int heartbeat_count = 0;
    
    while(true){
        // run in a 500Hz loop, no need to go faster
        if(_CP0_GET_COUNT() > 48000){
            _CP0_SET_COUNT(0);
            expander_pin_7 = get_expander();
            set_expander(0, !(expander_pin_7 >> 7));
            
            // heartbeat LED
            heartbeat_count++;
            if(heartbeat_count > 500){
                heartbeat_count = 0;
                user_LED = !user_LED;
            }
        }
    }
}