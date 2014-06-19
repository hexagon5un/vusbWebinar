
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */
#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include "usbdrv.h"
#include "descriptor.h"		/* HID descriptor */

#define LED_DDR        DDRB
#define LED_PORT       PORTB
#define LED            PB0

#define BUTTON         PD3
#define BUTTON_PORT    PORTD
#define BUTTON_PIN     PIND

#define SCROLLFACTOR   1  // can increase for faster scrolling 
