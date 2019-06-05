#include "config_bits.h"
#include "PIC_config.h"
#include "ili9341.h"
#include <stdio.h>

int main(){
    board_setup();
    SPI1_init();
    LCD_init();
    LCD_clearScreen(ILI9341_GREEN);
    LCD_drawRectangle(10,10, 200, 200, ILI9341_PURPLE);
    
    
    while(true){
        // 10Hz cycles
        if(_CP0_GET_COUNT() > 2400000){
            _CP0_SET_COUNT(0);
        }
    }
    
}