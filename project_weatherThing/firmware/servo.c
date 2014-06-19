
#include "servo.h"

void initServo(){
                   /* Set up Timer1 (16bit) to give a pulse every 20ms */
                             /* Use Fast PWM mode, counter max in ICR1 */
  TCCR1A |= (1 << WGM11);
  TCCR1B |= (1 << WGM12) | (1 << WGM13);
  TCCR1B |= (1 << CS11);                              /* /8 prescaling */
  ICR1 = 20000 * F_CPU / 8 / 1000000;  /* TOP value = 20ms * 12MHz / 8 */
  TCCR1A |= (1 << COM1A1);              /* Direct output on PB1 / OC1A */
  DDRB |= (1 << PB1);                            /* set pin for output */
  OCR1A = 1500;  	                   /* init in the middle for servo */ 
}

void setServo(uint16_t pulse){
/* Sets the servo to the correct position */ 
	LED_PORT_OUTPUT ^= _BV(LED_BIT);
	OCR1A = pulse;
}
