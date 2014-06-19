/* WeatherThing 
   A minimal V-USB Custom Control Transfer Demo */ 

int main(void) {

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
