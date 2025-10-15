# ESPHome Device Setup

These scripts and configuration contain the main configuration and
components for custom ESPHome hardware.  When flashed on to a device,
they can be given WiFi credentials through a captive portal, and
adopted into ESPHome dashboard automatically.

## Using a device once it's flashed or factory-reset

- Power on device
- Connect to the AP created by the device
- Visit 192.168.4.1 and configure WiFi credentials
- Visit ESPHome and adopt the device

## ESP based modules

### Build .bin for flashing via existing Tasmota:

    make build-gosund-dimmer

### Flash via serial

    make flash-gosund-dimmer TARGET=/dev/ttyUSB0

This maintains WiFi config.  If you want to erase it to get back to a
fresh provisioned state, erase first:

    pipenv run esptool.py --chip esp8266 -p /dev/ttyUSB0 erase_flash

### Flash via OTA over existing esphome firmware

    make flash-gosund-dimmer TARGET=gosund-dimmer-aabbcc.local

## ELEGRP DPR10 Dimmer Switch

This has a BK7231N module (not ESP32 or ESP8266), but there's support
in ESPHome for it through LibreTiny.

Unfortunately the firmware it was delivered with is too new to be
reflashed OTA with either tuya-convert or tuya-cloudcutter, so it
needs to be opened and flashed via serial.

### Hardware connections

Remove screws on the back.

Connect a Micro1v8 or MicroFTX configured for 3.3V as follows:

![](images/elegrp-dpr10-pinout.jpg)

    Micro1v8    DPR10
    GND         GND (G hole near Tuya MCU)
    OUT         TX (TX test point near Tuya MCU)
    IN          RX (RX test point near Tuya MCU)
    V           Tap onto 3.3V rail that goes into Wifi module

### Firmware

#### Kickstart

Get [kickstart-bk7231n-2025-05-28.uf2](https://github.com/libretiny-eu/esphome-kickstart/releases/download/v25.05.28/kickstart-bk7231n-2025-05-28.uf2)
from https://github.com/libretiny-eu/esphome-kickstart/releases/tag/v25.05.28

Flash with

    uv run --with ltchiptool,wxpython,zeroconf ltchiptool flash write -d /dev/serial/by-id/... kickstart-bk7231n-2025-05-28.uf2

Right after running that command, briefly touch GND to the exposed CEN finger on the Wifi module

After flashing, you should see a "kickstart-bk7231n" WiFI appear.
Connect, and configure it to connect to BACONHOME

#### OTA FW

Flash via esphome (through esphome OTA API) with e.g.:

    make flash-elegrp-dimmer-dpr10 TARGET=kickstart-bk7231n.home
    make flash-elegrp-dimmer-dpr10 TARGET=elegrp-dimmer-dpr10-128b7d.home
    make flash-elegrp-dimmer-dpr10 TARGET=elegrp-dimmer-dpr10-128b7d.bacon
