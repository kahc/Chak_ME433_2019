#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include <stdio.h>

#include "config_bits.h"


#include "ili9341.h"
#include "i2c_master_noint.h"
#include "ov7670.h"
#include<sys/attribs.h>  // __ISR macro

#define DIR1 LATAbits.LATA10
#define DIR2 LATAbits.LATA7
#define USER PORTBbits.RB4


// declarations
int got_image_recently, OC1_temp, OC4_temp;

// 1kHz interrupt. OC1RS and OC4RS are changed ONLY HERE
void __ISR(_TIMER_4_VECTOR, IPL5SOFT) motor_control(void) {
    IFS0bits.T4IF = 0;
    
    if(got_image_recently){
        OC1RS = OC1_temp;
        OC4RS = OC4_temp;
    }
    else{
        OC1RS = 0;
        OC4RS = 0;
    }
    
    if(_CP0_GET_COUNT() > (48000000/2/4)){
        got_image_recently = 0;
    }
}

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
    TRISAbits.TRISA7 = 0; // DIR2
    DIR2 = 1;
    TRISAbits.TRISA10 = 0; // DIR1
    DIR1 = 1;
    TRISBbits.TRISB4 = 1; // USER
    
    // OC1 is B15, goes with DIR1
    RPB15Rbits.RPB15R = 0b0101;
    
    // OC4 is A4, goes with DIR2
    RPA4Rbits.RPA4R = 0b0101;
    
    // use Timer 3 for PWM
    T3CONbits.TCKPS = 0; // Timer prescaler N=1 (1:1)
    PR3 = 2399; // PR = PBCLK / N / desiredF - 1
    TMR3 = 0; // initial TMR count is 0
    OC1CONbits.OCM = 0b110; // PWM mode without fault pin; other OC1CON bits are defaults
    OC4CONbits.OCM = 0b110;
    OC1CONbits.OCTSEL = 1; // User Timer3
    OC4CONbits.OCTSEL = 1;
    OC1RS = 0; // duty cycle
    OC1R = 0; // initialize before turning OC1 on; afterward it is read-only
    OC4RS = 0;
    OC4R = 0;
    T3CONbits.ON = 1; // turn on Timer
    OC1CONbits.ON = 1; // turn on OC1
    OC4CONbits.ON = 1;
    
    // LCD uses SPI1: A0 is SDO, A1 is SDI, B5 is CST, B14 is SCK1, A9 is DC, B7 is CS
    SPI1_init();
    LCD_init();
    LCD_clearScreen(ILI9341_DARKGREEN);
    
    // Camera uses C0-7, C8 as OC2, A8 as INT3/PCLK, B13 as INT2/HREF, C9 as INT1/VSYNC, and I2C1
    i2c_master_setup();
    ov7670_setup();
    
    // B3 is available as SCL2, B2 is available as SDA2
    
    // setup Timer 4 for ISR
	T4CONbits.TCKPS = 0b100;		// prescaler 16
	PR4 = 2999;				// 48Mhz/(16*(2999+1)) = 1000Hz
	TMR4 = 0;
	T4CONbits.ON = 1;

	// ISR setup
	IPC4bits.T4IP = 5;
	IPC4bits.T4IS = 0;
	IFS0bits.T4IF = 0;
	IEC0bits.T4IE = 1;
    
    got_image_recently = 0;
    OC1_temp = 0;
    OC4_temp = 0;
}

int main() {

    __builtin_disable_interrupts();

    startup();

    __builtin_enable_interrupts();
    
    int I = 0;
    char message[50];
    char com_message[50];
    unsigned char d[2000]; // this is the raw camera data, both brightness and color
    unsigned char bright[1000]; // this is for just the brightness data
    
    while(1) {
        
        I++;
        sprintf(message,"I = %d   ", I);
        drawString(140,82,message);
        
        // horizontal read
        /*
        int c = ov7670_count_horz(d);
        sprintf(message, "c = %d   ",c);
        drawString(140,92,message); // there are 290 rows
        
        int x = 0, x2 = 1;
        int y = 0;
        int dim = 0;
        for(x = 0; x < c/2; x++, x2=x2+2){
            dim = d[x2]>>3;
            bright[x] = d[x2];
            for(y=0;y<32;y++){
                if (y == dim){
                    LCD_drawPixel(y+30,x,ILI9341_BLACK);
                }
                else {
                    LCD_drawPixel(y+30,x,ILI9341_WHITE);
                }
            }
        }
        */
        
        // vertical read
        __builtin_disable_interrupts();
        
        int c = ov7670_count_vert(d);
        
        __builtin_enable_interrupts();
        got_image_recently = 1;
        _CP0_SET_COUNT(0);
        
        sprintf(message, "c = %d   ",c);
        drawString(140,92,message);
        
        int x = 0, x2 = 0;
        int y = 0;
        int dim = 0;
        for(x = 0; x < c/2; x++, x2=x2+2){
            dim = d[x2]>>3;
            bright[x] = d[x2];
            for(y=0;y<32;y++){
                if (y == dim){
                    LCD_drawPixel(x,y+30,ILI9341_BLACK);
                }
                else {
                    LCD_drawPixel(x,y+30,ILI9341_WHITE);
                }
            }
        }
        
        // at this point, bright has c/2 values in it, each value is a brightness of a pixel
        // loop through bright and calculate where the middle of the dip or bump is
        // then set the motor speed and direction to follow the line
        int i = 0;
        int sum = 0;
        int sumR = 0;
        int com = 0;
        int avg = 0;
        // find the average brightness
        for (i=0;i<c/2;i++){
            sum = sum + bright[i];
        }
        avg = sum / c/2;
        // threshold and center of mass calculation
        sum = 0;
        for (i=0;i<c/2;i++){
            if (bright[i]<avg){
                // count this pixel
                LCD_drawPixel(i,30,ILI9341_BLUE); // visualize which pixels we're counting
                sum = sum + 255;
                sumR = sumR + 255*i;
            }
            else {
                LCD_drawPixel(i,30,ILI9341_WHITE);
                // don't count this pixel
            }
        }
        // only use com if the camera saw some data
        if (sum>10){
            com = sumR/sum;
        }
        else {
            com = c/2/2;
        }
        // draw the center of mass as a bar
        for(y=0;y<32;y++){
            LCD_drawPixel(com,y+30,ILI9341_RED);
        }
        
        
        int speed = 0;
        float e = 0;
        // try to keep com at c/2/2 using the motors
        DIR1 = 0; // depending on your motor directions these might be different
        DIR2 = 1;
        // if com is less than c/2/2, then slow down the left motor, full speed right motor
        // if com is more than c/2/2, then slow down right motor, full speed left motor
        // things to play with: the slope of the line, the value that determines when the motor is not full speed
        int max_OC = 600;
        float kP = 1;
        // if too much error, slow down immediately
        
        if (com < c/2/2){
            e = (c/2/2 - com)/(float)(c/2/2);
            speed = max_OC - kP*max_OC*e; // when the com is all the way over, the motor is all off
            sprintf(com_message,"e: %4.2f speed = %d   ", e, speed);
            drawString(140,102,com_message);
            if(speed > max_OC){
                speed = max_OC;
            }
            if(speed < 0){
                speed = 0;
            }
            OC4_temp = max_OC;
            OC1_temp = speed;
        }
        else {
            e = (com - c/2/2)/(float)(c/2/2);
            speed = max_OC - kP*max_OC*e; // when the com is all the way over, the motor is all off
            sprintf(com_message,"e: %4.2f speed = %d   ", e, speed);
            drawString(140,102,com_message);
            if(speed > max_OC){
                speed = max_OC;
            }
            if(speed < 0){
                speed = 0;
            }
            OC4_temp = speed;
            OC1_temp = max_OC;
        }

    }
}

