#ifndef MOTOR_CONTROL_H    /* Guard against multiple inclusion */
#define MOTOR_CONTROL_H

#define DIR1 LATAbits.LATA10
#define DIR2 LATAbits.LATA7

void motor_init();

#endif
