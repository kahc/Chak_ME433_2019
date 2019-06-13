#include <xc.h>
#include "motor_control.h"
#include<sys/attribs.h>  // __ISR macro

int counter = 0;

void motor_init(){
    TRISAbits.TRISA7 = 0; // DIR2
    DIR2 = 1;
    TRISAbits.TRISA10 = 0; // DIR1
    DIR1 = 1;
    
    TRISBbits.TRISB3 = 0;
    
    // timer for ISR
	T3CONbits.TCKPS = 0;		// prescaler 1
	PR3 = 47999;				// 48Mhz/(47999+1) = 100Hz
	TMR3 = 0;
	T3CONbits.ON = 1;

	// ISR setup
	IPC3bits.T3IP = 5;
	IPC3bits.T3IS = 0;
	IFS0bits.T3IF = 0;
	IEC0bits.T3IE = 1;

	// timer and PWM setup
	T2CONbits.TCKPS = 0;		// prescaler 1
	PR2 = 2399;					// 48Mhz/2400 = 20kHz
	TMR2 = 0;
	OC1CONbits.OCTSEL = 1;
	OC1CONbits.OCM = 0b110;
	OC1RS = 1199;
 	OC1R = 0;
 	T2CONbits.ON = 1;
  	OC1CONbits.ON = 1;
    
}

void __ISR(_TIMER_3_VECTOR, IPL5SOFT) motor_control(void) {

IFS0bits.T3IF = 0;

// how many times has the interrupt occurred?
    if (counter > 99) {
        DIR1 = !DIR2;
        DIR2 = !DIR2;
        counter = 0;
    }
    counter++;
// set the duty cycle and direction pin

    
    LATBbits.LATB3 = !LATBbits.LATB3;

}