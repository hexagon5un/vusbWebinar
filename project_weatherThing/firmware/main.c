
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */

#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include "usbdrv.h"
#include "servo.h"

#define CMD_SET_SERVO       0x42
#define CMD_RELAX           0x01

/* ------------------------------------------------------------------------- */
/* ----------------------------- USB interface ----------------------------- */
/* ------------------------------------------------------------------------- */

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
	usbRequest_t    *rq = (void *)data;
	if(rq->bRequest == CMD_SET_SERVO){
		setServo((rq->wValue).word);
		return 0;                       /* tell the driver no data stage */
	} else if (rq->bRequest == CMD_RELAX){
		servoOff();
		return 0;
	}
	return 0;   /* default for not implemented requests: no data stage   */
}

/* ------------------------------------------------------------------------- */

int main(void)
{
	usbInit();
	usbDeviceDisconnect();  /* enforce re-enumeration */
	_delay_ms(250);
	usbDeviceConnect();
	sei();

	initServo();		
	LED_PORT_DDR |= _BV(LED_BIT);   /* make the LED bit an output */
	for(;;){                /* main event loop */
		usbPoll();
	}
}

/* ------------------------------------------------------------------------- */
