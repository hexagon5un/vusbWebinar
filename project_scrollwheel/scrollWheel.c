/*  scrollWheel   */
#include "scrollWheel.h"

/* This function used to respond to Vendor-Specific Control commands 
   We're not using any, so just returning 0. */ 
usbMsgLen_t usbFunctionSetup(uchar data[8]){
	return 0;  
}

int main(void){

	/* Reconnect and re-enumerate */
	usbInit();
	usbDeviceDisconnect();  
	_delay_ms(250);
	usbDeviceConnect();
	sei();

	LED_DDR |= (1 << LED);
	BUTTON_PORT |= (1<<BUTTON);

	while(1){			
		usbPoll();
		if(usbInterruptIsReady()){
			if (bit_is_clear(BUTTON_PIN, BUTTON)){ /* button is pressed */ 
				LED_PORT |= (1<<LED);
				reportBuffer.dWheel = -1 * SCROLLFACTOR;
			} else {
				reportBuffer.dWheel = 0;
				LED_PORT &= ~(1<<LED);
			}
			usbSetInterrupt((void *)&reportBuffer, sizeof(reportBuffer));
		}    
	} /* endless while */
}    /* mainloop */

