#include "config_bits.h"
#include "PIC_config.h"
#include "ili9341.h"
#include <stdio.h>

int main(){
    board_setup();
    SPI1_init();
    LCD_init();
    LCD_clearScreen(ILI9341_GREEN);
    
    int progress_percent = 0;
    char message[20];
    float fps = 10;
    int message_counter = 0;
    int fps_message_counter = 0;
    char fps_message[20];
    
    
    
    while(true){
        // 10Hz cycles
        if(_CP0_GET_COUNT() > 2400000){
            _CP0_SET_COUNT(0);
            
            sprintf(message, "Hello World %d%%", progress_percent);
            while(message[message_counter]){
                LCD_drawChar(28 + 5*message_counter, 32, ILI9341_RED, message[message_counter]);
                message_counter++;
            }
            message_counter = 0;
            
            sprintf(fps_message, "fps: %2.3f", fps);
            while(fps_message[fps_message_counter]){
                LCD_drawChar(50 + 5*fps_message_counter, 200, ILI9341_BLUE, fps_message[fps_message_counter]);
                fps_message_counter++;
            }
            fps_message_counter = 0;
            
            LCD_drawLine(10+progress_percent, 10+progress_percent, 50, 60, ILI9341_PURPLE);
            
            fps = 48000000/(float)(_CP0_GET_COUNT()+2400000);
            
            progress_percent++;
            if(progress_percent > 100){
                progress_percent = 0;
                LCD_clearScreen(ILI9341_GREEN);
            }
        }
    }
    
}