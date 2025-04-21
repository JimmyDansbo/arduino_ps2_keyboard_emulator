ARDUINO := /home/jimmyd/Downloads/arduino-1.8.19/arduino 

all:
	$(ARDUINO) --upload --port /dev/ttyUSB0 --board arduino:avr:nano:cpu=atmega328old ps2_keyboard/ps2_keyboard.ino

compile:
	$(ARDUINO) --verify --board arduino:avr:nano:cpu=atmega328old ps2_keyboard/ps2_keyboard.ino

