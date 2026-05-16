# LZN Drone Thing

Firmware for an ESP32-based FPV drone gate/checkpoint system.

The goal of this project is to build an interactive track for small FPV drones.  
Each gate detects when something passes through it and controls two LED strips.  
Multiple gates, for example `gate01`, `gate02`, ..., `gate10`, are intended to work together over WiFi and be controlled from a laptop by a Python application.

The ESP32 firmware should stay as simple as possible.  
Most of the race logic, timing, ordering and state management is planned to be handled by the external Python controller.

## Project status

This project is currently under development.

Current focus:
- ESP32 firmware structure
- WiFi connection
- OTA updates
- local configuration files
- optical sensor handling
- LED strip control

Planned features:
- Switch to esp32 c3 seeed for smaller hardware
- communication with a Python controller
- multiple gates in one WiFi network
- gate state synchronization
- lap / checkpoint timing
- flight order validation
- race logic handled mostly on the python side

## System concept
The final system is planned to consist of multiple independent gates.
