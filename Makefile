devices=$(foreach yaml,$(sort $(wildcard *.yaml)),$(basename $(yaml)))

.PHONY: all
all: help

.PHONY: help
help:
	@echo "Makefile targets:"
	@printf "  %-25s Build all\n" "build"
	@$(foreach d,$(devices),printf "  %-25s Check config for $d\n" "config-$d" ;)
	@$(foreach d,$(devices),printf "  %-25s Build $d.bin\n" "build-$d" ;)
	@$(foreach d,$(devices),printf "  %-25s Build+flash $d\n" "flash-$d" ;)
	@printf "For flashing, set TARGET=hostname.home or TARGET=/dev/ttyUSB0\n"

define device_rules
.PHONY: config-$1
config-$1:
	uv run esphome config $1.yaml

.PHONY: build-$1
build-$1: $1.bin
$1.bin:
	pipenv run esphome compile $1.yaml
	cp -vf .esphome/build/$1/.pioenvs/$1/firmware.bin $1.bin
build:: build-$1
clean::
	rm -f $1.bin

.PHONY: flash-$1
flash-$1:
	pipenv run esphome run $1.yaml $(if $(TARGET),--device $(TARGET))
endef
$(foreach d,$(devices),$(eval $(call device_rules,$d)))

clean::
	rm -rf .esphome

.PHONY: ctrl
ctrl:
	git add -u
	git commit --amend --no-edit
	git push -f
	pipenv run esphome compile gosund-dimmer.yaml

#flash:
#	pipenv run esptool.py --chip esp8266 -p /dev/ttyUSB0 write_flash 0x0 gosund-dimmer.bin
