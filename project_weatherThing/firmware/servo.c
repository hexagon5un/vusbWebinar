
#include "servo.h"

void initServo(){

}

void setServo(uint16_t pulse){
/* Sets the servo to the correct position */ 
	if (pulse & 1){
		LED_PORT_OUTPUT &= ~_BV(LED_BIT);
	} else {
		LED_PORT_OUTPUT |= _BV(LED_BIT);
	}
}
