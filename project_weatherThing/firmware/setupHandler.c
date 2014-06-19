/* WeatherThing 
   A minimal V-USB Custom Control Transfer Demo */ 

#define CMD_SET_SERVO       0x42
#define CMD_RELAX           0x01

usbMsgLen_t usbFunctionSetup(uchar data[8]) {
	usbRequest_t    *rq = (void *)data;
	if(rq->bRequest == CMD_SET_SERVO){
		setServo((rq->wValue).word);
	} else if (rq->bRequest == CMD_RELAX){
		servoOff();
	}
	return 0;              /* sets return data length: no data stage */
}
