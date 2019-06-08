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
    char IMU_raw_data[14];
    signed short IMU_data[7];
    char message[20];
    int bar_x, bar_y;

    
    int heartbeat_count = 0;
    
    while(true){
        // 20Hz cycles
        if(_CP0_GET_COUNT() > 1200000){
            _CP0_SET_COUNT(0);
            
            I2C_read_multiple(reg_OUT_TEMP_L, IMU_raw_data, 14);
            int data_counter;
            for(data_counter = 0; data_counter<7; data_counter++){
                IMU_data[data_counter] = (IMU_raw_data[2*data_counter+1] << 8) | IMU_raw_data[2*data_counter];
            }
            
            
            // scale IMU data to fit screen, and to adjust sensitivity
            bar_x = -IMU_data[4]/90;
            bar_y = -IMU_data[5]/70;
            bar_x = min(120, bar_x);
            bar_x = max(-120, bar_x);
            bar_y = min(160, bar_y);
            bar_y = max(-160, bar_y);
            
            // draw bars
            LCD_drawRectangle(120, 120+bar_x, 155, 165, ILI9341_PURPLE);
            LCD_drawRectangle(115, 125, 160, 160+bar_y, ILI9341_PURPLE);
            
            // sprintf(message, "accel_x: %d", IMU_data[4]);
            // LCD_drawWord(150, 50, ILI9341_RED, message);
            
            heartbeat_count++;
            if(heartbeat_count > 5){
                heartbeat_count = 0;
                user_LED = !user_LED;
                
                // clear screen every heartbeat
                LCD_clearScreen(ILI9341_GREEN);
            }
        }
    }
    
}