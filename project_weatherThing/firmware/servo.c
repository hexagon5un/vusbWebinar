
#include "servo.h"

void initServo(){

}

void setServo(uint16_t pulse){
/* Sets the servo to the correct position */ 
	LED_PORT_OUTPUT ^= _BV(LED_BIT);
}
