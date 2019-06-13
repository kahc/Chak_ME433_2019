#include <xc.h>           // processor SFR definitions
#include "config_bits.h"
#include<sys/attribs.h>  // __ISR macro
#include "motor_control.h"
#include "ili9341.h"
#include <stdio.h>

#define USER PORTBbits.RB4

void startup() {
    // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);

    // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;

    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;

    // disable JTAG to get pins back
    DDPCONbits.JTAGEN = 0;

    // pin definitions
    ANSELA = 0;
    ANSELB = 0;
    TRISBbits.TRISB4 = 1; // USER
    
    motor_init();
    SPI1_init();
    LCD_init();
    
    // LCD uses SPI1: A0 is SDO, A1 is SDI, B5 is CST, B14 is SCK1, A9 is DC, B7 is CS
    
    
    
    // Camera uses C0-7, C8 as OC2, A8 as INT3/PCLK, B13 as INT2/HREF, C9 as INT1/VSYNC, and I2C1
    
    // B3 is available as SCL2, B2 is available as SDA2
    
    
    
    
}

int main() {

    __builtin_disable_interrupts();

    startup();

    __builtin_enable_interrupts();
    
    // simulate camera data, sort of
    char R[240] = {0};
    char G[240] = {0};
    char B[240] = {0};
    R[100] = 10;
    G[50] = 20;
    B[150] = 30;
    
    LCD_clearScreen(ILI9341_GREEN);
       
    while(1) {
        mass_center_vertical_plot(10, R);
        mass_center_vertical_plot(100, G);
        mass_center_vertical_plot(200, B);
        

    }
}