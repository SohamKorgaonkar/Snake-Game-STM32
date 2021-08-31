# The Snake Game
## What game ..?
Yes, this is the recreation of the old snake game made to run on an STM32 F4 Discovery Board. It is just a bare bones snake game for now. Features like snake and food sprites will be added in the future.

## Requirments
 - STM32 F4 Discovery Board ( Or any F4 based ARM Board)
 - 4 Push Buttons
 - 1.3 inch I2C OLED display board
 - A way to connect it all together
 - Keil IDE with STM pack installed
 - Patience

## Connections

### OLED Display

 - VCC to 5V 
 - GND to GND
 - SDA to PB7
 - SCL to PB6
### Push Buttone
 - Down Button PA1, GND
 - Up Button PA4, GND
 - Right Button PA2, GND
 - Left Button PA3, GND

## Steps To Recreate

 1. Make the connections
 2. Open the Keil Project and Flash it
 3. Reset the board

