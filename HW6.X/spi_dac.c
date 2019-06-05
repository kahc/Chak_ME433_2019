// defining functions for spi_dac.h

#include "spi_dac.h"

void init_SPI1(){
    SPI1CON = 0;              // turn off the spi module and reset it
    
    // setup cs pin to A4, initially high
    TRISAbits.TRISA4 = 0;
    dac_cs = 1;
    
    // don't setup sdi pin until we need it
    //SDI1Rbits.SDI1R = 0b0001;
    
    // setup sdo pin to B5
    RPB5Rbits.RPB5R = 0b0011;
    
    SPI1BUF;                  // clear the rx buffer by reading from it
    SPI1BRG = 1;            // fastest baud rate possible
    SPI1STATbits.SPIROV = 0;  // clear the overflow bit
    SPI1CONbits.CKE = 1;      // data changes when clock goes from hi to lo (since CKP is 0)
    SPI1CONbits.MSTEN = 1;    // master operation
    SPI1CONbits.ON = 1;       // turn on spi 1
    
}

unsigned char spi_io(unsigned char write) {
  SPI1BUF = write;
  while(!SPI1STATbits.SPIRBF) { // wait to receive the byte
    ;
  }
  return SPI1BUF;
}

void setVoltage(char channel, unsigned int voltage){
    unsigned short command = 0;
    command = channel << 15;    // move channel information to the front
    command = command | 0b0111000000000000;    // default settings
    command = command | (voltage&0x0FFF);        // voltage information
    
    dac_cs = 0;
    spi_io(command >> 8);
    spi_io(command & 0x00FF);
    dac_cs = 1;
}
