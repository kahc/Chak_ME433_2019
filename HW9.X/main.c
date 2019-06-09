#include "config_bits.h"
#include "PIC_config.h"
#include "ili9341.h"
#include "i2c_IMU.h"
#include <stdio.h>

int main(){
    board_setup();
    SPI1_init();
    LCD_init();
    init_IMU();
    
    LCD_clearScreen(ILI9341_GREEN); 
    
    unsigned short touch_x_raw, touch_y_raw;
    unsigned short touch_x, touch_y;
    unsigned int touch_z;
    char message[40];
  
    int heartbeat_count = 0;
    
    while(true){
        // 5Hz cycles
        if(_CP0_GET_COUNT() > 1200000){
            _CP0_SET_COUNT(0);
            
            touchscreen_read(&touch_x_raw, &touch_y_raw, &touch_z);
            
            touch_x = (touch_x_raw - 400) * 240.0/3500.0;
            touch_y = -(touch_y_raw - 3900) * 360.0/3500.0;
            
            
            sprintf(message, "touch_x: %d, touch_y: %d, touch_z: %d        ", touch_x, touch_y, touch_z);
            LCD_drawWord(5, 50, ILI9341_RED, message);
            
            heartbeat_count++;
            if(heartbeat_count > 5){
                heartbeat_count = 0;
                user_LED = !user_LED;
            }
        }
    }
    
}