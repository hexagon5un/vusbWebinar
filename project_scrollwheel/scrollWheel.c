/*  scrollWheel   */
#include <avr/io.h>
// #include <avr/sleep.h> 
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */
#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include "usbdrv.h"
#include "descriptor.h"		/* HID descriptor */

#define LEDTIME       1 	/* ms */
#define SCROLLFACTOR  1

#define LED_DDR        DDRB
#define LED_PORT       PORTB
#define LED            PB0

typedef struct{
    uchar   buttonMask;
    char    dx;
    char    dy;
    char    dWheel;
}report_t;

static report_t reportBuffer;

usbMsgLen_t usbFunctionSetup(uchar data[8]){
  return 0;   /* default for not implemented requests: return no data back to host */
}

// Globals

/*static inline void pollPins(void){*/
  /*thisPinState = INPIN & _BV(IN1);*/
     /*if (thisPinState != lastPinState){ [> seen change <]*/

       /*if (thisPinState){		[> rising <]*/
	 /*if (INPIN & _BV(IN2)){	[> lagging <]*/
	   /*if (cycleState == twoFirstDown){ [> round trip with two leading <]*/
		 /*scrollAccumulator++;*/
		 /*blinkGreen();*/
	   /*}*/
	   /*cycleState = twoFirstUp;*/
	 /*}*/
	 /*else {			[> leading <]*/
	   /*if (cycleState == oneFirstDown){ [> round trip with one leading <]*/
		 /*scrollAccumulator--;*/
		 /*blinkRed();*/
	   /*}*/
	   /*cycleState = oneFirstUp;*/
	 /*}*/
       /*}		       [> end pin 1 rising <]*/
       /*else{			        [> dropping <]*/
	 /*if (INPIN & _BV(IN2)){	[> leading <]*/
	   /*if (cycleState == oneFirstUp){ [> round trip with one leading <] */
		 /*scrollAccumulator--;*/
		 /*blinkRed();*/
	   /*}*/
	   /*cycleState = oneFirstDown;  */
	 /*}*/
	 /*else {			[> lagging <]*/
	   /*if (cycleState == twoFirstUp){ [> round trip with two leading <] */
		 /*scrollAccumulator++;*/
		 /*blinkGreen();*/
	   /*}*/
	   /*cycleState = twoFirstDown; */
	 /*}*/
       /*} [> end pin1 falling <]*/
     /*}	 [> end pin change <]*/
     /*lastPinState = thisPinState;*/
/*}*/


int main(void){

	int8_t scrollAccumulator;		/* again, overkill? */
  /* This block below from the USB example */
  usbInit();
  usbDeviceDisconnect();  
  _delay_ms(250);
  usbDeviceConnect();
  sei();

  LED_DDR |= (1 << LED);

  while(1){			

    /*pollPins();		*/
    /* Block from USB-mouse example */
    usbPoll();
    if(usbInterruptIsReady()){
      reportBuffer.dWheel = scrollAccumulator * SCROLLFACTOR;
      usbSetInterrupt((void *)&reportBuffer, sizeof(reportBuffer));
      scrollAccumulator = 0;
    }    
  } /* endless while */
}    /* mainloop */
