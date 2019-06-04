// using the MCP 23008 with the PIC32

#ifndef I2C_PIN_EXPANDER_H    /* Guard against multiple inclusion */
#define I2C_PIN_EXPANDER_H

// pin definitions


// function declarations
void init_expander();
void set_expander(char pin, char level);
char getExpander();

#endif