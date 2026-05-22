# Interactive FPV drone track

Firmware for an ESP32-based FPV drone gate/checkpoint system.

The goal of this project is to build an interactive track for small FPV drones.  
Each gate detects when something passes through it and controls two LED strips.  
Multiple gates, for example `gate01`, `gate02`, ..., `gate10`, are intended to work together over WiFi and be controlled from a laptop by a Python application.

The ESP32 firmware should stay as simple as possible.  
Most of the race logic, timing, ordering and state management is planned to be handled by the external Python controller.

## Project status

This project is currently under development.
As in its current state the main funcionality of controlling the LEDs based on detecting and object passing throug the gate it is complete. 
Each gate can operate on its own, the animations and detection range can be confiugured by user. There is no way to control the whole course consting of multiple gates.


Planned features:
- communication with a Python controller
- multiple gates in one WiFi network
- gate state synchronization
- lap / checkpoint timing
- flight order validation
- race logic handled mostly on the python side

## System concept
The final system is planned to consist of multiple independent gates. Flight order, time stamps and lap times will be programmable from a simple pc application. 

