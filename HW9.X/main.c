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
    char coordinates[20];
    char counter_message[10];
    int counter = 0;
    int counter_change = 0;
  
    int heartbeat_count = 0;
    
    while(true){
        // 10Hz cycles
        if(_CP0_GET_COUNT() > 2400000){
            _CP0_SET_COUNT(0);
            
            touchscreen_read(&touch_x_raw, &touch_y_raw, &touch_z);
            
            touch_x = (touch_x_raw - 400) * 240.0/3500.0;
            touch_y = -(touch_y_raw - 3900) * 320.0/3500.0;
            
            sprintf(message, "raw_x: %d, raw: %d, touch_z: %d        ", touch_x_raw, touch_y_raw, touch_z);
            LCD_drawWord(5, 50, ILI9341_RED, message);
            sprintf(coordinates, "touch_x: %d, touch_y: %d      ", touch_x, touch_y);
            LCD_drawWord(5, 60, ILI9341_RED, coordinates);
            sprintf(counter_message, "%d    ", counter);
            LCD_drawWord(115, 160, ILI9341_NAVY, counter_message);
            
            LCD_drawRectangle(115, 126, 145, 156, ILI9341_PURPLE);
            LCD_drawRectangle(115, 126, 175, 186, ILI9341_PURPLE);
            LCD_drawRectangle(120, 121, 147, 154, ILI9341_BLACK);
            LCD_drawRectangle(117, 124, 149, 150, ILI9341_BLACK);
            LCD_drawRectangle(117, 124, 179, 180, ILI9341_BLACK);
            
            // when pressed, change is buffered into counter_change
            if(touch_z > 500){
                if(abs(touch_x - 120) < 7 & abs(touch_y - 150) < 7){
                    counter_change = 1;
                }
                if(abs(touch_x - 120) < 7 & abs(touch_y - 180) < 7){
                    counter_change = -1;
                }
            }
            // if not pressed, do counter_change and then clear it
            else{
                counter = counter + counter_change;
                counter_change = 0;
            }
            
            heartbeat_count++;
            if(heartbeat_count > 5){
                heartbeat_count = 0;
                user_LED = !user_LED;
            }
        }
    }
    
}