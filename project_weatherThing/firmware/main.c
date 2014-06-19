
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */

#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include "usbdrv.h"
#include "servo.h"

#define CMD_SET_SERVO       0x42
/* ------------------------------------------------------------------------- */
/* ----------------------------- USB interface ----------------------------- */
/* ------------------------------------------------------------------------- */

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
	usbRequest_t    *rq = (void *)data;
	if(rq->bRequest == CMD_SET_SERVO){
		setServo(123);
		return 0;                       /* tell the driver to send 1 byte */
	}
	return 0;   /* default for not implemented requests: return no data back to host */
}

/* ------------------------------------------------------------------------- */

int main(void)
{
	usbInit();
	usbDeviceDisconnect();  /* enforce re-enumeration, do this while interrupts are disabled! */
	_delay_ms(250);
	usbDeviceConnect();
	LED_PORT_DDR |= _BV(LED_BIT);   /* make the LED bit an output */
	sei();

	for(;;){                /* main event loop */
		usbPoll();
	}
}

/* ------------------------------------------------------------------------- */
