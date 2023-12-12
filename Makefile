.PHONY: all
all: venv
	@echo TODO

.PHONY: venv
venv: .venv/pyvenv.cfg

.venv/pyvenv.cfg:
	mkdir -p .venv
	pipenv install
	[ -e $@ ] || ( rm -rf .venv ; echo Failed to make pyvenv.cfg ; false )

#flash:
#	pipenv run esptool.py --chip esp8266 -p /dev/ttyUSB0 write_flash 0x0 gosund-dimmer.bin
