#include "PIC_config.c"

int main(){
    board_config();
    while(1) {
	// use _CP0_SET_COUNT(0) and _CP0_GET_COUNT() to test the PIC timing
	// remember the core timer runs at half the sysclk
        while(!user_button){;}
        if(_CP0_GET_COUNT() > 12000){
            _CP0_SET_COUNT(0);
            user_LED = !user_LED;
        }
        
    }
}