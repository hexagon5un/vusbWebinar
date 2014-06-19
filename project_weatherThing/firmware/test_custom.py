import usb
import time

OUT_TYPE = usb.util.build_request_type(usb.util.CTRL_OUT, usb.util.CTRL_TYPE_VENDOR, usb.util.CTRL_RECIPIENT_DEVICE)

## Command set
CMD_SET_SERVO = 0x42

dev = usb.core.find(idVendor=0x6666, idProduct=0xbeef)

servo =[1201, 1500, 1701, 1702, 1500]
## Toggle LED, read status, then echo some data
for pulseLength in servo:
    dev.ctrl_transfer(
            bmRequestType = OUT_TYPE, 
            bRequest      = CMD_SET_SERVO, 
            wValue        = pulseLength
            )
    time.sleep(2)

    dev.ctrl_transfer(
            bmRequestType = OUT_TYPE, 
            bRequest      = 0x01, 
            wValue        = 0 
            )
 
