.PHONY: run build upload monitor reset clean

BAUD_RATE ?= 115200
PORT ?= /dev/ttyUSB0

run: upload monitor

build:
	pio run

upload:
	pio run -t upload

monitor:
	pio device monitor -b $(BAUD_RATE) -p $(PORT)

reset:
	pio device reset -p $(PORT)

clean:
	pio run -t clean
