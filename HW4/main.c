#include "PIC_config.h"
#include "config_bits.h"
#include "spi_dac.h"

int main(){
    board_setup();
    init_SPI1();
    
    while(1) {
        while(!user_button){;}
        if(_CP0_GET_COUNT() > 12000){
            setVoltage(0,1000);
        }
        if(_CP0_GET_COUNT() > 24000){
            _CP0_SET_COUNT(0);
            setVoltage(0,3000);
        }
        
    }
}