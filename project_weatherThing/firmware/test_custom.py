import usb
import time

OUT_TYPE = usb.util.build_request_type(usb.util.CTRL_OUT, usb.util.CTRL_TYPE_VENDOR, usb.util.CTRL_RECIPIENT_DEVICE)

## Command set
CMD_SET_SERVO = 0x42

dev = usb.core.find(idVendor=0x16c0, idProduct=0x05dc)

## Toggle LED, read status, then echo some data
for i in range(10):
    dev.ctrl_transfer(
            bmRequestType = OUT_TYPE, 
            bRequest      = CMD_SET_SERVO, 
            wValue        = 0
            )
    time.sleep(0.2)
