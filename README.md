# Game Tetris on Arduino

This project builds a **one-player [_Tetris_](https://en.wikipedia.org/wiki/Tetris) game on Arduino**, where a **8*8 LED matrix** is used as the monitor and an **infrared sensor** as the signal receiver. 

This project realizes almost all the fundamental rules of the traditional _Tetris_ game. The player can control the falling pieces to move left, move right or rotate. Thanks to infrared receiver, the controlling signal is quite sensitive even from a large distance. 

![]

## Usage

1. To begin, the following are needed:

   - [Arduino](https://www.arduino.cc/) IDE
   - Arduino board (I use Arduino Uno)
   - 8*8 LED matrix
   - Infrared receiver
   - Remote control
   - Eight 1kΩ resistors
   - [IRremote](https://www.arduinolibraries.info/libraries/i-rremote) library

1. Complete the circuit. 

   ![circuit](https://github.com/Yangliu20/arduino-tetris/blob/main/docs/images/electronics.jpg)

1. Change the codes of controlling signal to the codes of your remote control. 

1. Upload the sketch to Arduino. 
