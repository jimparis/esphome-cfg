devices=$(foreach yaml,$(sort $(wildcard *.yaml)),$(basename $(yaml)))

.PHONY: all
all: help

.PHONY: help
help:
	@echo "Makefile targets:"
	@printf "  %-20s Build all\n" "build"
	@$(foreach d,$(devices),printf "  %-20s Build $d\n" "build-$d" ;)
	@$(foreach d,$(devices),printf "  %-20s Build+flash $d\n" "flash-$d" ;)

define device_rules
.PHONY: flash-$1
build-$1: venv
	pipenv run esphome compile $1.yaml
build:: build-$1
flash-$1: venv
	pipenv run esphome upload $1.yaml
endef
$(foreach d,$(devices),$(eval $(call device_rules,$d)))

.PHONY: venv
venv: .venv/pyvenv.cfg

.venv/pyvenv.cfg:
	mkdir -p .venv
	pipenv install
	[ -e $@ ] || ( rm -rf .venv ; echo Failed to make pyvenv.cfg ; false )

#flash:
#	pipenv run esptool.py --chip esp8266 -p /dev/ttyUSB0 write_flash 0x0 gosund-dimmer.bin
