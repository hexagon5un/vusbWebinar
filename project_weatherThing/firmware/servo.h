
/* ------------------------------------------------------------------------- */
/* ----------------------------- Servo Functions --------------------------- */
/* ------------------------------------------------------------------------- */

#include <avr/io.h>

// For great blinkage
#define LED_PORT_DDR        DDRB
#define LED_PORT_OUTPUT     PORTB
#define LED_BIT             0

void initServo(void);
/* Sets up timer1 to send servo pulses */

void setServo(uint16_t pulse);
/* Sets the servo to the specified position */ 


void servoOff(void);

