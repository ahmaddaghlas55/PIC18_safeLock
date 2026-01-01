/*
 * File:   Basic Circuit.c
 * Author: Nicolas Jackson & Aitana Bonmati
 *
 * Created on October 20, 2024, 8:37 PM
 */

// PIC18F4620 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1H
#pragma config OSC = XT         // Oscillator Selection bits (XT oscillator)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = ON         // Watchdog Timer Enable bit (WDT enabled)
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = ON      // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = ON         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-003FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (004000-007FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (008000-00BFFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (00C000-00FFFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-003FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (004000-007FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (008000-00BFFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (00C000-00FFFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (004000-007FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (008000-00BFFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (00C000-00FFFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0007FFh) not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include "lcd_x8.h"
#include "keypad_3x4.h"
#include "my_ser.h"
#include "my_adc.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

#define _XTAL_FREQ   4000000UL 
#define STARTVALUE 3036
#define BUZZER PORTCbits.RC3
#define LED_RED PORTEbits.RE1
#define LED_GREEN PORTEbits.RE0
#define RESET_PASSWORD '*'
#define ENTER '#'
#define CORRECT_PASSWORD "2580"
#define CORRECT_VOLT 1.7
unsigned int count = 0;
unsigned char password[24];
int pass_index = 0;
char seconds = 10;
char Buffer[64];
float volt;
void displayTime(void);
char flag_first = 1;
char mask_mode = 1;   // 1 = show '*',   0 = show real digits
float lastVolt = -1;



void delay_ms(unsigned int n)
{
    int i;
    for (i=0; i < n; i++){
        __delaywdt_ms(1); 
    }
}
void delay_sec(unsigned int n){
    int i;
    for(i = 0; i < n; ++i){
        delay_ms(1000);
    }
}

void initPorts(void) {
    ADCON0 = 0;
    ADCON1 = 0b00001101; //2 analog channels
    LATA = LATB = LATC = LATD = LATE = 0;
    TRISA = 0xFF; // All inputs
    TRISB = 0xFF; // all pushbuttons are inputs
    TRISD = TRISE = 0; // All outputs
    TRISC = 0b10000000; // RX input , others output
}
void smallBuzz(void) {
    BUZZER = 1;
    __delay_ms(50);
    BUZZER = 0;
}



void check_password(){
    lcd_gotoxy(1, 2);
    lcd_putc('\f');
    float rounded_volt = ((int)(volt * 10 + 0.5)) / 10.0;
    if(!strcmp(password, CORRECT_PASSWORD) && rounded_volt == CORRECT_VOLT){
        lcd_puts("Welcome!");
        BUZZER = 1;
        LED_GREEN = 1;
        LED_RED = 0;
        delay_ms(3000);
        BUZZER = 0;
        lcd_putc('\f');
        lcd_puts("door open");
                
        while(keypad_scan() != RESET_PASSWORD){
            delay_ms(10);
        }
        lcd_putc('\f');
    }
    else {
        lcd_puts("Wrong Password!");
        BUZZER = 1;
        delay_ms(850);
        BUZZER = 0;
        delay_ms(650);
        BUZZER = 1;
        delay_ms(850);
        BUZZER = 0;        
        //lcd_gotoxy(1, 2);
        //lcd_putc('\f');
        while (seconds > 0){
            displayTime();
            delay_ms(1000);
            seconds--;
            
        }
        lcd_putc('\f');

        seconds = 10;
        
        
    }
    pass_index = 0;
    delay_ms(500);
    strcpy(password, "");
    flag_first = 1;
}


void displayTime(void) {
        //INTCONbits.GIE = 0;
        delay_ms(3);
        lcd_gotoxy(1, 2);
        sprintf(Buffer, "%02d:%02d:%02d", 0, 0, seconds);
        lcd_puts(Buffer);
        strcpy(Buffer, "");
        //INTCONbits.GIE = 1; 


}

void main(void) {
    INTCON = 0;
    initPorts();
    setupSerial();
    //char Buffer[64]; //commented this 
    unsigned char ReceivedChar = 0;
    unsigned char SendToSerial = 0;
    keypad_init();
    unsigned char key;

    lcd_init();
    init_adc_no_lib();
    lcd_putc('\f');
    
    send_string_no_lib((unsigned char *) "AHMAD & HOSNIE\r\n"); // sending to the UART
    
    lcd_gotoxy(1, 1);
    sprintf(Buffer, "AHMAD & HOSNIE");
    lcd_puts(Buffer);
    BUZZER = 0;
    delay_ms(2000); // delay 20s
    lcd_putc('\f');
    
    char x;
    int p1 = 1;
    int p2 = 2;
    
    while (1){
        CLRWDT();
        delay_ms(3);
        int adc_value1 = read_adc_raw_no_lib(1);
        int adc_value1 = read_adc_raw_no_lib(1);
        volt = (adc_value1 / 1023.0) * 5.0;
        delay_ms(3);
        int adc_value0 = read_adc_raw_no_lib(0);
        int adc_value0 = read_adc_raw_no_lib(0);
        float temp = (adc_value0 / 1023.0) * 500.0;
        delay_ms(3);
        sprintf(Buffer, "T=%2.1f V=%4.1f", temp, volt);
        lcd_gotoxy(1, 1);
        lcd_puts(Buffer);
                if (fabs(volt - lastVolt) > 0.05) {//
            smallBuzz();//
            lastVolt = volt;//
        }//

        LED_RED = 1;
        LED_GREEN = 0;
        BUZZER = 0;
        // Scan the keypad for key press
        char key = keypad_scan();
        if (key != '\0') {  // Check if a valid key is pressed
            smallBuzz();
            if(flag_first == 0 && pass_index == 0){
                lcd_gotoxy(1, 2);
                lcd_putc('\f');
            }
            if(key == ENTER) {
                password[pass_index] = '\0';
                check_password();
                
                continue;
            }
            
            password[pass_index++] = key;
            lcd_gotoxy(pass_index, 2);
            lcd_putc(key); // Display key on LCD
        }
        else{
            if(flag_first == 1){
                sprintf(Buffer, "Enter Password...");
                lcd_gotoxy(1, 2);
                lcd_puts(Buffer);
                flag_first = 0;
            }
        }

        delay_ms(50);
    }




    return;
}
