# PIC18 SafeLock Project

## Overview
This project implements a **digital safe lock system** using a **PIC18F4620 microcontroller**.  
It combines a **keypad**, **LCD display**, **LED indicators**, **buzzer**, and **ADC voltage input** to control access securely.

## Features
- Users enter a **password** via a 3x4 keypad.  
- The system checks the password **and a voltage condition** to unlock.  
- **Visual feedback**:
  - Green LED and "Welcome!" message when correct.  
  - Red LED and "Wrong Password!" message when incorrect.  
- **Buzzer alerts** for keypresses and incorrect attempts.  
- Password input can be **masked** with asterisks.  
- Displays **real-time voltage and temperature** readings on the LCD.  
- System waits for reset (`*`) after successful unlock.

## Components Used
- PIC18F4620 Microcontroller  
- 16x2 LCD  
- 3x4 Keypad  
- LEDs (Red & Green)  
- Buzzer  
- Analog sensors (for voltage/temperature measurement)

## Usage
1. Power the microcontroller and connect all components.  
2. Enter the **password** on the keypad.  
3. The system will check the password and voltage:
   - Correct: Green LED lights, door opens (simulation), "Welcome!" displayed.  
   - Incorrect: Red LED lights, buzzer sounds, countdown shown.  
4. Reset by pressing `*` after unlocking.

## Password & Voltage
- Default password: `2580`  
- Required voltage: `1.7V` (analog input channel)

## Notes
- This project is written in **C for MPLAB X IDE** using XC8 compiler.  
- Designed as a **learning and demonstration project** for embedded systems and digital access control.  

## Author
- Ahmad Daghlas & Hosine Badran
