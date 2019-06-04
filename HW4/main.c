#include "PIC_config.h"
#include "config_bits.h"
#include "spi_dac.h"
#include <math.h>

int main(){
    board_setup();
    init_SPI1();
    
    // 5Hz triangle wave and 10Hz sine wave, updated 1kHz rate
    // create 200 point array with a counter
    
    int triangle_wave[200];
    int sine_wave[200];
    int i;
    for(i = 0; i<200; i++){
        // being lazy, triangle wave can goes to 99*41=4056
        if(i<100){
            triangle_wave[i] = i*40;
        }
        else{
            triangle_wave[i] = 4000-(i-100)*40;
        }
        
        // sine has period of 200
        sine_wave[i] = 2000*(sin((i/100.0)*2*M_PI)+1);
    }
    
    i = 0;
    
    while(1) {
        if(_CP0_GET_COUNT() > 24000){
            _CP0_SET_COUNT(0);
            i++;
            if(i>199){
                i=0;
            }
            setVoltage(0,triangle_wave[i]);
            setVoltage(1,sine_wave[i]);
        }
    }
}