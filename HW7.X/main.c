#include "config_bits.h"
#include "PIC_config.h"
#include "ili9341.h"
#include <stdio.h>

int main(){
    board_setup();
    SPI1_init();
    LCD_init();
    
    LCD_clearScreen(ILI9341_GREEN);
    LCD_drawRectangle(30, 150, 50, 175, ILI9341_PURPLE);
    LCD_drawWord(50, 100, ILI9341_RED, "testing");
    int heartbeat_count = 0;
    
    while(true){
        // 10Hz cycles
        if(_CP0_GET_COUNT() > 2400000){
            _CP0_SET_COUNT(0);
            
            heartbeat_count++;
            if(heartbeat_count > 10){
                heartbeat_count = 0;
                user_LED = !user_LED;
            }
        }
    }
    
}