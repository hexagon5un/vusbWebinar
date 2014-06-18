import usb
import time

READ_TYPE = usb.util.build_request_type(usb.util.CTRL_IN, usb.util.CTRL_TYPE_VENDOR, usb.util.CTRL_RECIPIENT_DEVICE)
SET_TYPE = usb.util.build_request_type(usb.util.CTRL_OUT, usb.util.CTRL_TYPE_VENDOR, usb.util.CTRL_RECIPIENT_DEVICE)

## Command set
ECHO = 0 
SET_LED = 1  
READ_LED = 2 

dev = usb.core.find(idVendor=0x16c0, idProduct=0x05dc)

## Toggle LED, read status, then echo some data
dev.ctrl_transfer(
        bmRequestType = SET_TYPE, 
        bRequest      = SET_LED, 
        wValue        = 0
        )
result = dev.ctrl_transfer(
        bmRequestType   = READ_TYPE,
        bRequest        = READ_LED,
        data_or_wLength = 1
        )
print result
time.sleep(1)

dev.ctrl_transfer(
        bmRequestType = SET_TYPE,
        bRequest      = SET_LED,
        wValue        = 1
        )
result = dev.ctrl_transfer(
        bmRequestType   = READ_TYPE,
        bRequest        = READ_LED,
        data_or_wLength = 1
        )
print result
time.sleep(1)

dev.ctrl_transfer(
        bmRequestType = SET_TYPE,
        bRequest      = SET_LED,
        wValue        = 0
        )
result = dev.ctrl_transfer(
        bmRequestType   = READ_TYPE,
        bRequest        = READ_LED,
        data_or_wLength = 1
        )
print result
time.sleep(1)

dev.ctrl_transfer(
        bmRequestType = SET_TYPE,
        bRequest      = SET_LED,
        wValue        = 123
        )
result = dev.ctrl_transfer(
        bmRequestType   = READ_TYPE,
        bRequest        = READ_LED,
        data_or_wLength = 1
        )
print result
time.sleep(1)

print dev.ctrl_transfer(
        bmRequestType    = READ_TYPE,
        bRequest         = ECHO,
        wValue           = 21,
        data_or_wLength  = 1
        )
print dev.ctrl_transfer(
        bmRequestType    = READ_TYPE,
        bRequest         = ECHO,
        wValue           = 42,
        data_or_wLength  = 1
        )
