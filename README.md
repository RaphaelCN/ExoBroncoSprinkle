# Exo Bronco Sprinkle
Arduino Code for Exo Bronco Avionics PCB

**11/1/23 Status:**
- Pressure sensor running on Core 0, Radio working on Core 1.
- Successful data transmission at 10ms/100Hz
- Currently working on GPS integration

# File Explanations
_SprinkleTest.ino_
  - Main program running on the board
  - Sets up peripherals and begins tasks for data acquistion and transmission

_debug_custom.json_
  - Automatically generated config file for custom board

_Header Files (.h)_
  - Definitions for peripherals
  - Should have 1 setup method, 1 read method, and "get" methods in order to access the read data in SprinkleTest.ino
  - DataPacket.h is a different header, just defines a DataPacket object, allows us to transmit one variable containing all the various data

_Source Code (.cpp)_
  - Implementations for each peripheral header file.
  - DataPacket.h does not have an accompanying .cpp file

# Working on Code
_Setting up Arduino IDE_
  - First make sure you have the ESP32 boards installed on your IDE: https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/
  - When choosing a board to flash to, select "ESP32S3 Dev Module"
  - Ensure programming settings are like below:
  - 
