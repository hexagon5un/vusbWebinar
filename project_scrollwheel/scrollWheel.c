
/*  scrollWheel   */

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h> 
// #define F_CPU 12000000UL 	/* F7 DF F9/01 */
/* 12 MHz crystal with 2x 22pF caps to ground */
// avrdude -c usbtiny -p m88 -v -F -U lfuse:w:0xf7:m
#include <util/delay.h>
#include "usbSupport.h"		/* includes bunch of crap from USB mouse demo */

#define LEDTIME       1 	/* ms */
#define SCROLLFACTOR  1
// Pin Defines           ------------------------------------------------

#define LED_REGISTER   DDRC
#define LED_PORT       PORTC
#define REDLED         PC2
#define GREENLED       PC3

#define IN_PORT        PORTD
#define IN_REGISTER    DDRD
#define INPIN          PIND
#define IN1            PD6
#define IN2            PD5


// Globals
uint8_t thisPinState; /* yes, it's a waste of memory. we have plenty. */
uint8_t lastPinState;
int scrollAccumulator;		/* again, overkill? */

enum CYCLE {		   /* Where we are in the quadrature */
  oneFirstDown,	    /* pin one is low  while pin two is high */
  oneFirstUp,	    /* pin one is high while pin two is low */
  twoFirstDown,	    /* pin one is low  while pin two is low */
  twoFirstUp	    /* pin one is high while pin two is high */
}  cycleState;

// ISRs


// Inits                 ------------------------------------------------

static inline void blinkGreen(void){
  LED_PORT |= _BV(GREENLED);
  _delay_ms(LEDTIME);
  LED_PORT &= ~_BV(GREENLED);
}

static inline void blinkRed(void){
  LED_PORT |= _BV(REDLED);
  _delay_ms(LEDTIME);
  LED_PORT &= ~_BV(REDLED);
}

static inline void initLED(void){
  LED_REGISTER |= _BV(GREENLED) | _BV(REDLED);
  LED_PORT &= ~_BV(GREENLED);
  LED_PORT &= ~_BV(REDLED);
  /* blink to show working */
  _delay_ms(100);
  blinkRed(); 
  _delay_ms(100);  
  blinkGreen();  
  _delay_ms(100);
  blinkRed(); 
  _delay_ms(100);  
  blinkGreen();  
}

static inline void pollPins(void){
  thisPinState = INPIN & _BV(IN1);
     if (thisPinState != lastPinState){ /* seen change */

       if (thisPinState){		/* rising */
	 if (INPIN & _BV(IN2)){	/* lagging */
	   if (cycleState == twoFirstDown){ /* round trip with two leading */
	     scrollAccumulator++;
	     blinkGreen();
	   }
	   cycleState = twoFirstUp;
	 }
	 else {			/* leading */
	   if (cycleState == oneFirstDown){ /* round trip with one leading */
	     scrollAccumulator--;
	     blinkRed();
	   }
	   cycleState = oneFirstUp;
	 }
       }		       /* end pin 1 rising */
       else{			        /* dropping */
	 if (INPIN & _BV(IN2)){	/* leading */
	   if (cycleState == oneFirstUp){ /* round trip with one leading */ 
	     scrollAccumulator--;
	     blinkRed();
	   }
	   cycleState = oneFirstDown;  
	 }
	 else {			/* lagging */
	   if (cycleState == twoFirstUp){ /* round trip with two leading */ 
	     scrollAccumulator++;
	     blinkGreen();
	   }
	   cycleState = twoFirstDown; 
	 }
       } /* end pin1 falling */
     }	 /* end pin change */
     lastPinState = thisPinState;
}


int main(void){

  /* This block below from the USB example */
  uchar   i;
  wdt_enable(WDTO_1S);
  usbInit();
  usbDeviceDisconnect();  
  i = 0;
  while(--i){             /* fake USB disconnect for > 250 ms */
    wdt_reset();
    _delay_ms(1);
  }
  usbDeviceConnect();
  sei();

  initLED();

  while(1){			

    /* Get motion state */
    pollPins();		

    /* Block from USB-mouse example */
    wdt_reset();		
    usbPoll();
    if(usbInterruptIsReady()){
      reportBuffer.dWheel = scrollAccumulator * SCROLLFACTOR;
      usbSetInterrupt((void *)&reportBuffer, sizeof(reportBuffer));
      scrollAccumulator = 0;
    }    

  } /* endless while */
}    /* mainloop */
