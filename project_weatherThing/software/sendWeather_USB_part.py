
import usb.core 
import usb.util
import time

commandDict = {'setServo':0x42, 'relax':0x01}

requestType = usb.util.build_request_type(
        usb.util.CTRL_OUT, 
        usb.util.CTRL_TYPE_VENDOR, 
        usb.util.CTRL_RECIPIENT_DEVICE
        )

dev = usb.core.find(idVendor=0x6666, idProduct=0xbeef)

dev.ctrl_transfer(  ## Turn servo on, set position
        bmRequestType = requestType, 
        bRequest      = commandDict['setServo'], 
        wValue        = servoPulseLength
        )

time.sleep(1)

dev.ctrl_transfer(  ## Turn servo off, save power
        bmRequestType = requestType, bRequest = commandDict['relax'] )


