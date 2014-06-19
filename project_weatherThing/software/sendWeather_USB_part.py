## Stripped-down Example of Computer-VUSB Device Communication

import usb.core 
import usb.util

## Now the USB side
commandDict = {'setServo':0x42}

requestType = usb.util.build_request_type(
        usb.util.CTRL_OUT, 
        usb.util.CTRL_TYPE_VENDOR, 
        usb.util.CTRL_RECIPIENT_DEVICE
        )

dev = usb.core.find(idVendor=0x16c0, idProduct=0x05dc)

dev.ctrl_transfer(
        bmRequestType = requestType, 
        bRequest      = commandDict['setServo'], 
        wValue        = servoPulseLength
        )
