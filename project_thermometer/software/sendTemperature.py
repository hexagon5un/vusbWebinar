## Stripped-down Example of Computer-VUSB Device Communication

import urllib2
import json
import datetime

# weatherURL = 'http://api.openweathermap.org/data/2.5/weather?q=Munich,DE&units=metric'
# temperature = weather['main']['temp'] 
# temperature = float(temperature)   ## Kelvin to Celcius conversion

forecastURL = 'http://api.openweathermap.org/data/2.5/forecast/daily?q=Munich,DE&cnt=7&mode=json&units=metric'
webpage = urllib2.urlopen(forecastURL).read()
j = json.JSONDecoder()
weather = j.decode(webpage)

today = weather['list'][0]['weather'][0]['main']
tomorrow = weather['list'][1]['weather'][0]['main']

pointerDict = {'Clear':1400, 'Clouds':1800, 'Rain':2300}

## Press button on device, initializes web search for data, transfers it over.
## States?

## Software Machine:
## Waiting: On press -> fetch
## Fetching: on complete -> update
## Updating: wait for complete -> wait

## AVR Machine:
## Waiting -> on press transmit
## Transmitting -> wait for update
## Update -> receive data, move motor -> waiting

