/*
 * File:   keypad_3x4.c
 * Author: Ahmad
 *
 * Created on December  2, 2025, 1:31 AM
 */

#include "keypad_3x4.h"
#include <xc.h>
#define _XTAL_FREQ 4000000UL

// Keymap for the 4x3 keypad
const char keymap[4][3] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
};

/*
 9  6  3
 8  5  2
 7  4  1
 */

//initialize PORTB for keypad scanning
void keypad_init(void) {
    TRISB = 0b01111000; // RB0-RB2 as column outputs, and others as row inputs, RB7 is free to use however you like
    INTCON2bits.RBPU = 0; //enable internal pull-ups on PORTB
}

//scan the keypad and return the pressed key
char unsigned keypad_scan(void) {
        C1 = 0; C2 = 1; C3 = 1;
    if (R1 == 0) {
        while (R1 == 0); //wait until key is released
        return '1';
    }
    if (R2 == 0) {
        while (R2 == 0);
        return '4';
    }
    if (R3 == 0) {
        while (R3 == 0);
        return '7';
    }
    if (R4 == 0) {
        while (R4 == 0);
        return '*';
    }

    C1 = 1; C2 = 0; C3 = 1;
    if (R1 == 0) {
        while (R1 == 0);
        return '2';
    }
    if (R2 == 0) {
        while (R2 == 0);
        return '5';
    }
    if (R3 == 0) {
        while (R3 == 0);
        return '8';
    }
    if (R4 == 0) {
        while (R4 == 0);
        return '0';
    }

    C1 = 1; C2 = 1; C3 = 0;
    if (R1 == 0) {
        while (R1 == 0);
        return '3';
    }
    if (R2 == 0) {
        while (R2 == 0);
        return '6';
    }
    if (R3 == 0) {
        while (R3 == 0);
        return '9';
    }
    if (R4 == 0) {
        while (R4 == 0);
        return '#';
    }

    return '\0';
}
