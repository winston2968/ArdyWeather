# ArdyWeather

A brief description and starting instruction to run ArdyWeather on your ArduinoMEGA2560. 

## Installation

Tu run ```ArdyWeather``` you will need ```ArduinoIDE``` or ```arduino-cli``` on your machine. Personnally I prefer arduino-cli tool. It allows me to code on every IDE and then compile and upload code with a few commands. 

### Install Arduino core 

```bash 
# Update and Install core 
arduino-cli core update-index 
arduino-cli core install arduino:avr
```

### Find Arduino MEGA port 

```bash 
arduino-cli board list
```

It will list all arduino port found which correspond to something like ```/dev/ttyACM0```
or ```/dev/cu.usbmodem...```

### Compile and upload 

From the folder ```arduino_code``` containning the ```arduino_code.ino``` file you can compile the file with: 
```bash 
arduino-cli compile --fqbn arduino:avr:mega arduino_code.ino
```

and then upload it on the arduino you want with: 

```bash 
arduino-cli upload -p /dev/ttyACM0 --fqbn arduino:avr:mega arduino_code
```

You can also use the ```Makefile```to compile and upload ```.ino``` files on cards. 

## Features 

For this project, I use RadioController and TempHumSensor : 
- https://www.gotronic.fr/art-capteur-de-temperature-et-d-humidite-grove-101020074-23841.htm
- https://github.com/Seeed-Studio/Grove_Temper_Humidity?tab=readme-ov-file
