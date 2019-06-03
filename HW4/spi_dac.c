// defining functions for spi_dac.h

#include "spi_dac.h"

void init_SPI1(){
    SPI1CON = 0;              // turn off the spi module and reset it
    
    // setup cs pin to B0
    TRISBbits.TRISB0 = 0;
    
    // setup sdi pin to B1
    SDI1Rbits.SDI1R = 0b0010;
    
    // setup sdo pin to A2
    RPA2Rbits.RPA2R = 0b0011;
    
    
}

char SPI1_IO(char write){
    
}

void setVoltage(char channel, int voltage){
    
}
