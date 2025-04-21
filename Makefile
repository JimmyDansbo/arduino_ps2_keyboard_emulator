ARDUINO := /home/jimmyd/Downloads/arduino-1.8.19/arduino 
PORT := /dev/ttyUSB0
BOARD := arduino:avr:nano:cpu=atmega328old
TERMINAL := /usr/bin/gtkterm

all:
	$(ARDUINO) --upload --port $(PORT) --board $(BOARD) ps2_keyboard/ps2_keyboard.ino

compile:
	$(ARDUINO) --verify --board $(BOARD) ps2_keyboard/ps2_keyboard.ino

terminal:
	$(TERMINAL) --port $(PORT)
