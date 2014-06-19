
/*  scrollWheel   */

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h> 
// #define F_CPU 12000000UL 	/* F7 DF F9/01 */
/* 12 MHz crystal with 2x 22pF caps to ground */
// avrdude -c usbtiny -p m88 -v -F -U lfuse:w:0xf7:m
#include <util/delay.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */
#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include "usbdrv.h"
#include "descriptor.h"		/* HID descriptor */

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

typedef struct{
    uchar   buttonMask;
    char    dx;
    char    dy;
    char    dWheel;
}report_t;

static report_t reportBuffer;
static uchar    idleRate;   /* repeat rate for keyboards, never used for mice */

usbMsgLen_t usbFunctionSetup(uchar data[8]){
  usbRequest_t    *rq = (void *)data;
  
  /* The following requests are never used. But since they are required by
   * the specification, we implement them in this example.
   */
  if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS){    /* class request type */
    //    DBG1(0x50, &rq->bRequest, 1);   /* debug output: print our request */
    if(rq->bRequest == USBRQ_HID_GET_REPORT){  /* wValue: ReportType (highbyte), ReportID (lowbyte) */
      /* we only have one report type, so don't look at wValue */
      usbMsgPtr = (void *)&reportBuffer;
      return sizeof(reportBuffer);
    }
    else if(rq->bRequest == USBRQ_HID_GET_IDLE){
      usbMsgPtr = &idleRate;
      return 1;
    }
    else if(rq->bRequest == USBRQ_HID_SET_IDLE){
      idleRate = rq->wValue.bytes[1];
    }
  }
  else{
    /* no vendor specific requests implemented */
  }
  return 0;   /* default for not implemented requests: return no data back to host */
}


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
