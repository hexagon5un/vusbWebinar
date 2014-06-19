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
