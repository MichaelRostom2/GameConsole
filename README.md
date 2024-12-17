# GameConsole
A CS1600 Project for Fall 2024. By Henry Earnest, Michael Rostom, and Nathan Kim

## Materials needed
- 1 Arduino Uno R4 Wifi
- 1 ILI9341 TFT LCD display
- 2 Buttons
- 1 Joystick


## Wiring
#### Screen: 
- Wire up DC into Arduino's pin 8.
- Wire up RST into Arduino's pin 9.
- Wire up CS into Arduino's pin 10.
- Wire up MOSI into Arduino's pin 11.
- Wire up SCK into Arduino's pin 13

#### Home Button:
- Wire up into Arduino's pin 3.

#### Reset Button:
- Wire up into Arduino's pin 2.


#### Joystick: 
- Wire up VRX into Arduino's A0.
- Wire up VRY into Arduino's A1.


## Dependencies
#### Arduino Dependencies: 
- Arduino_GFX Library by moononournation. (tested with version 1.5.0)

#### Python Dependencies: 
- pySerial. (tested with version 3.4)

## How to run
Open Arduino IDE and get the port at which Arduino is connected at(bottom right).
For mac it looks something like: "dev/tty.usbmodemF412FA9F9FB02"
For windows it looks something like: "COM6"
replace python file 52 with your port.

Compile and upload code using Arduino IDE(or other method of uploading code). Make sure Serial monitor is not opened on the Arduino IDE and run UART-Laptop.py in terminal and wait for it to say "Listening for Arduino...".
Then you can interact with console. Enjoy Ping and Dodge!


## FAQ
#### What do I do if I have Joystick Drift? 
You can calibrate the joystick by making sure offset values in GameConsole.ino are accurate so that pollInputs returns 0 for x and y when joystick is in the middle.



