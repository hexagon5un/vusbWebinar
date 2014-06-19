
import usb.core 
for device in usb.core.find(find_all=True):
    print "%04x:%04x" % (device.idVendor, device.idProduct)
