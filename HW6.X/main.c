#include "config_bits.h"
#include "PIC_config.h"
#include "ili9341.h"

int main(){
    board_setup();
    SPI1_init();
    LCD_init();
    LCD_clearScreen(ILI9341_GREEN);
    LCD_drawPixel(100,100, ILI9341_RED);
    
    int heartbeat_count = 0;
    
    while(true){
        if(_CP0_GET_COUNT() > 24000){
            _CP0_SET_COUNT(0);
            
            // heartbeat LED
            heartbeat_count++;
            if(heartbeat_count > 1000){
                heartbeat_count = 0;
                user_LED = !user_LED;
            }
        }
    }
    
}