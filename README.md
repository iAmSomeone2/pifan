# Pi Fan
A project for running and managing 3 and 4-pin fans connected to the Raspberry Pi's GPIO pins.

Currently, the entire project is being shifted over to C++.

## pifand
The daemon used to monitor system temps and activate the fan when the Pi needs to be cooled.

Additional features include:
* Optional fan speed monitoring
* Configurable target temperature.

## target_temp
A small program used for setting the target temperature of the system and restarting the pifand
process through Systemd.

## Pi Fan Control
The GTK+ app for managing the pifand daemon.

## Build Requirements

### Raspbian
* cmake
* pkg-config
* libjsoncpp-dev
* libpigpiod-if-dev
* libgtkmm-3.0-dev

### Arch Linux and Manjaro
* cmake
* pkgconf (included in base-devel group)
* jsoncpp
* python-pigpio ([AUR package](https://aur.archlinux.org/packages/python-pigpio))
* gtkmm3
