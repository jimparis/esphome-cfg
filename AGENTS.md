This is esphome configuration for various smart home devices.  See README.md for more details.

Validation
==========
To verify configuration for `foo.yaml`, run `UV_CACHE_DIR=.uv-cache make config-foo`.
After verifying configuration, test build with `UV_CACHE_DIR=.uv-cache make build-foo`.

Flashing
========
Typically I reflash existing devices using the "install" option in the
ESPHome dashboard.  For that to work, changes should first be
validated locally, then committed & pushed.  Do not commit .bin files.
