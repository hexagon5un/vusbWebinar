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
pointerDict = {'Clear':2500, 'Clouds':1700, 'Rain':1000}
servoPulseLength = pointerDict[tomorrowsWeather]

import usb.core 
import usb.util
import time

## Now the USB side
commandDict = {'setServo':0x42, 'relax':0x01}

requestType = usb.util.build_request_type(
        usb.util.CTRL_OUT, 
        usb.util.CTRL_TYPE_VENDOR, 
        usb.util.CTRL_RECIPIENT_DEVICE
        )

dev = usb.core.find(idVendor=0x6666, idProduct=0xbeef)
dev.ctrl_transfer(bmRequestType = requestType, bRequest = commandDict['setServo'],
        wValue = pointerDict['Clear']) 
time.sleep(1)
dev.ctrl_transfer(bmRequestType = requestType, bRequest = commandDict['setServo'],
        wValue = pointerDict['Rain'])
time.sleep(1)
dev.ctrl_transfer(bmRequestType = requestType, bRequest = commandDict['setServo'],
        wValue = pointerDict['Clouds']) 
time.sleep(1)

dev.ctrl_transfer(bmRequestType = requestType, 
        bRequest = commandDict['setServo'], wValue = servoPulseLength) 
time.sleep(1)

dev.ctrl_transfer( bmRequestType = requestType, bRequest = commandDict['relax'] )


