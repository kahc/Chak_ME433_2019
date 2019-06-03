#include "PIC_config.h"

void board_setup(){
    __builtin_disable_interrupts();

    // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);

    // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;

    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;

    // disable JTAG to get pins back
    DDPCONbits.JTAGEN = 0;

    // setup user_LED
    TRISAbits.TRISA0 = 0;
    
    //setup user_button
    TRISAbits.TRISA1 = 1;
    ANSELAbits.ANSA1 = 0;
    user_LED = false;
    
    __builtin_enable_interrupts();
}