# ESPHome Device Setup

These scripts and configuration contain the main configuration and
components for custom ESPHome hardware.  When flashed on to a device,
they can be given WiFi credentials through a captive portal, and
adopted into ESPHome dashboard automatically.

## Usage examples

### Build .bin for flashing via existing Tasmota:

    make build-gosund-dimmer

### Flash via serial

    make flash-gosund-dimmer TARGET=/dev/ttyUSB0

This maintains WiFi config.  If you want to erase it to get back to a
fresh provisioned state, erase first:

    pipenv run esptool.py --chip esp8266 -p /dev/ttyUSB0 erase_flash

### Flash via OTA over existing esphome firmware

    make flash-gosund-dimmer TARGET=gosund-dimmer-aabbcc.local

## Using a device once it's flashed

- Power on device
- Connect to the AP created by the device
- Visit 192.168.4.1 and configure WiFi credentials
- Visit ESPHome and adopt the device
