// Need Interrupt Endpoint
#define USB_CFG_HAVE_INTRIN_ENDPOINT    1

// Declare as HID Class
#define USB_CFG_DEVICE_CLASS        0
#define USB_CFG_INTERFACE_CLASS     3
#define USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH    52

// Identification Stuffs
#define  USB_CFG_VENDOR_ID       0x66, 0x66 
#define  USB_CFG_DEVICE_ID       0xfe, 0xca 

#define USB_CFG_VENDOR_NAME    'D', 'e', 'm', 'o' 
#define USB_CFG_VENDOR_NAME_LEN 4
#define USB_CFG_DEVICE_NAME     'S', 'c', 'r', 'o', 'l', 'l', ' ' , 'W', 'h', 'e', 'e', 'l', ' ', 'o', 'f', ' ', 'D', 'O', 'O','M','!'  
#define USB_CFG_DEVICE_NAME_LEN 21
