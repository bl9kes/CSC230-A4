# Assignment 4 – LED Control with Timers (CSC Fall 2022)

This project is based on an AVR microcontroller and demonstrates controlling LEDs using timers, interrupts, and pulse-width modulation (PWM). The provided skeleton code included protected "DO NOT TOUCH" sections for timer setup and global variables, while the student sections implement the assignment tasks.

## This code received a grade of 24.2/22 = 110%. 
 Produces the desired outcome as per this video.
 - https://www.youtube.com/watch?v=rWHe6r2pI_4


## Features Implemented

1. **`led_state(uint8_t LED, uint8_t state)`**  
   - Turns individual LEDs on or off.  
   - Maps four LEDs to specific bits of `PORTL`.  

2. **`SOS()`**  
   - Flashes LEDs to mimic the Morse code signal for "SOS" (`... --- ...`).  
   - Uses predefined light patterns and durations to sequence the LED blinks.  

3. **`glow(uint8_t LED, float brightness)`**  
   - Implements constant brightness control for an LED using PWM.  
   - Brightness is a fraction (`0.0 – 1.0`) of the duty cycle.  

4. **`pulse_glow(uint8_t LED)`**  
   - Produces a pulsing LED effect where brightness gradually increases and decreases.  
   - Controlled by the counters updated via timer interrupts.  

5. **`light_show()`** *(Bonus)*  
   - Runs a pre-programmed LED light sequence.  
   - Uses bit patterns and timing arrays for a dynamic LED animation.  
