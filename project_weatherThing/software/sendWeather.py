## Stripped-down Example of Computer-VUSB Device Communication

import urllib2
import json

## First the Internet side
forecastURL =  'http://api.openweathermap.org/data/2.5/forecast/daily'
forecastURL += '?q=Munich,DE&cnt=2&mode=json&units=metric'

webpage = urllib2.urlopen(forecastURL).read()
j = json.JSONDecoder()
weather = j.decode(webpage)

## The following navigates through the nested list/dict
##  structure that's returned
tomorrowsWeather = weather['list'][1]['weather'][0]['main']

## Result is a string: ('Clear', 'Clouds', or 'Rain')
print tomorrowsWeather

## Convert weather string to servo pulse length
pointerDict = {'Clear':1400, 'Clouds':1800, 'Rain':2300}
servoPulseLength = pointerDict[tomorrowsWeather]
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
