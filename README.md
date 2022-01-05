# Wavelogger2
Underwater Pressure and Temperature logger

This project is for making a pressure / temperature logger from commercially available parts. 
The loggers run approximately 2 weeks on a battery charge and can log at a frequency of ~20Hz down to once per 30 seconds. 
THe depth limit under water is ~300m, as set by the pressure sensor. 
Data is stored on a micro SD card.

The project is inspired by Luke Miller's Open Wave Height Logger project and copies heavily from his work.
Most electrical hardware is sourced from Adafruit and the base of code is derived from their example code for the adalogger hardware.
All pressure tolerant hardware is sourced from Blue Robotics. They also provide the library for interpreting the pressure and temperature sensors.

Included files here are:
1) operating instructions for the loggers (This includes a bill of materials if anyone wants to source and build their own)
2) clock setting software for the persistent clock - direct from Adafruit
3) Operating software for the logger in its logging state
4) CAD fies for the parts tray used inside the blue robotics pressure housing
5) An assembly diagram and instructions will be developed if anyone asks for them! ( This can include a more detailed bill of materials with purchasing links)
